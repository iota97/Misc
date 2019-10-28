/*

 Tabelle di verita' su openCL

 Compilazione:

	 c++ -O2 LogTab.cpp -o LogTab.out -lOpenCL

 File di input:

	26 variabili da A a Z

	Operazioni:
		
	AND &
	OR v
	NOT !
	IMP ->
	EQ <->

	Es:  A&!A, BvC, A<->!A, (AvB)->(C&D)

*/


#include <iostream>
#include <vector>
#include <string>

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

static const char *intro =
    "kernel void run(\n"
    "       global short *out\n"
    "       )\n"
    "{\n"
    "    size_t i = get_global_id(0);\n"
    "    bool A = i&(1 << 0);\n"
    "    bool B = i&(1 << 1);\n"
    "    bool C = i&(1 << 2);\n"
    "    bool D = i&(1 << 3);\n"
    "    bool E = i&(1 << 4);\n"
    "    bool F = i&(1 << 5);\n"
    "    bool G = i&(1 << 6);\n"
    "    bool H = i&(1 << 7);\n"
    "    bool I = i&(1 << 8);\n"
    "    bool J = i&(1 << 9);\n"
    "    bool K = i&(1 << 10);\n"
    "    bool L = i&(1 << 11);\n"
    "    bool M = i&(1 << 12);\n"
    "    bool N = i&(1 << 13);\n"
    "    bool O = i&(1 << 14);\n"
    "    bool P = i&(1 << 15);\n"
    "    bool Q = i&(1 << 16);\n"
    "    bool R = i&(1 << 17);\n"
    "    bool S = i&(1 << 18);\n"
    "    bool T = i&(1 << 19);\n"
    "    bool U = i&(1 << 20);\n"
    "    bool V = i&(1 << 21);\n"
    "    bool W = i&(1 << 22);\n"
    "    bool X = i&(1 << 23);\n"
    "    bool Y = i&(1 << 24);\n"
    "    bool Z = i&(1 << 25);\n"
    "    if(";

static char pre_cmd[65536];
static char cmd[65536];

static const char *outro  =
    ") out[0] = 1; else out[1] = 1;\n    }\n";

int main(int argc, char** argv) {

    if (argc < 2) {
	std::cout << "Sintassi: LogTab <input_file.txt>" << std::endl;
	return 1;
    }

    try {
	// Get list of OpenCL platforms.
	std::vector<cl::Platform> platform;
	cl::Platform::get(&platform);

	if (platform.empty()) {
	    std::cerr << "Support OpenCL non trovato" << std::endl;
	    return 1;
	}

	// Get first available GPU device which supports double precision.
	cl::Context context;
	std::vector<cl::Device> device;
	for(auto p = platform.begin(); device.empty() && p != platform.end(); p++) {
	    std::vector<cl::Device> pldev;

	    try {
		p->getDevices(CL_DEVICE_TYPE_GPU, &pldev);

		for(auto d = pldev.begin(); device.empty() && d != pldev.end(); d++) {
		    if (!d->getInfo<CL_DEVICE_AVAILABLE>()) continue;

		    std::string ext = d->getInfo<CL_DEVICE_EXTENSIONS>();

		    device.push_back(*d);
		    context = cl::Context(device);
		}
	    } catch(...) {
		device.clear();
	    }
	}

	// Create command queue.
	cl::CommandQueue queue(context, device[0]);

	FILE *fi = fopen (argv[1], "rb");

	if (!fi) {
		std::cout << "Errore nell'apertura del file" << std::endl;
		return 1;
	}

	// Crea il comando
	char* p = (char*) pre_cmd;
	char c;
    	while ((c = fgetc(fi)) != EOF) {

		if(c == '&') { // A&B   A&&B
			*p++ = '&';
			*p++ = '&';
		}

		else if(c == 'v') { // AvB   A||B
			*p++ = '|';
			*p++ = '|';
		}

		else if(c == '<') { // A<->B   A==B
			*p++ = '=';
			*p++ = '=';
			fgetc(fi);
			fgetc(fi);
		}

		else {
			*p++ = c;
		}
	}
	*++p = '\0';
	fclose(fi);

	p = (char*) pre_cmd;
	char *n = (char*) cmd;

	while ((c = *p++) != '\0') {// A->B   !A||(A&&B)

		*n++ = c;

		if (c == '-') {
			char A[4096];
			char B[4096];

			int A_par = 0;
			char* A_last = p-2;
			char* A_first = A_last;
			do {
				char a = *A_first;
				A_first--;
				if (a == ')')
					A_par++;
				if (a == '(')
					A_par--;

			} while(A_par != 0);

			for (int i = 1; i <= A_last-A_first; ++i) {
				A[i-1] = *(A_first+i);
			}

			int B_par = 0;
			char* B_first = p+1;
			char* B_last = B_first;
			do {
				char b = *B_last;
				B_last++;
				if (b == '(')
					B_par++;
				if (b == ')')
					B_par--;

			} while(B_par != 0);

			for (int i = 0; i < B_last-B_first; ++i) {
				B[i] = *(B_first+i);
			}

			n -= (A_last-A_first)+1;
			
			*n++ = '!';
			for (int i = 1; i <= A_last-A_first; ++i)
				*n++ = A[i-1];
			*n++ = '|';
			*n++ = '|';
			*n++ = '(';
			for (int i = 1; i <= A_last-A_first; ++i)
				*n++ = A[i-1];
			*n++ = '&';
			*n++ = '&';
			for (int i = 0; i < B_last-B_first; ++i)
				*n++ = B[i];
			*n++ = ')';
			p += B_last-B_first+1;
			
		}
	}
 
	char *source = (char*) malloc(sizeof(char)*(strlen(intro)+strlen(cmd)+strlen(outro)));
	strcpy(source, intro);
	strcpy(source+strlen(intro), cmd);
	int back = cmd[strlen(cmd)-1] == '\n';
	strcpy(source+strlen(intro)+strlen(cmd)-back, outro);

	// Compile OpenCL program for found device.
	cl::Program program(context, cl::Program::Sources(
		    1, std::make_pair(source, strlen(source))
		    ));

	try {
	    program.build(device);
	} catch (const cl::Error&) {
	    std::cerr
		<< "Errore nella compilazione OpenCL" << std::endl
		<< program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device[0])
		<< std::endl;
	    return 1;
	}

	cl::Kernel run(program, "run");

	std::vector<short> out(2);
	out[0] = 0;
	out[1] = 0;

	cl::Buffer OUT(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
		out.size() * sizeof(short), out.data());

	// Set kernel parameters.
	run.setArg(0, OUT);
	
	// Launch kernel on the compute device.
	queue.enqueueNDRangeKernel(run, cl::NullRange, 1<<20, cl::NullRange);

	// Get result back to host.
	queue.enqueueReadBuffer(OUT, CL_TRUE, 0, out.size() * sizeof(short), out.data());

	bool fal = out[1];
	bool tru = out[0];

	if (tru && fal)
		std::cout << "Opinione" << std::endl;
	else if (tru)
		std::cout << "Tautologia" << std::endl;
	else
		std::cout << "Paradosso" << std::endl;

    } catch (const cl::Error &err) {
	std::cerr
	    << "Errore OpenCL: "
	    << err.what() << "(" << err.err() << ")"
	    << std::endl;
	return 1;
    }

    return 0;
}
