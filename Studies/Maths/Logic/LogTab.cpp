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

	Es:  A&!A, BvC, !Av(A&B)

*/


#include <iostream>
#include <vector>
#include <string>

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

// Compute c = a + b.
static const char *intro =
    "#if defined(cl_khr_fp64)\n"
    "#  pragma OPENCL EXTENSION cl_khr_fp64: enable\n"
    "#elif defined(cl_amd_fp64)\n"
    "#  pragma OPENCL EXTENSION cl_amd_fp64: enable\n"
    "#else\n"
    "#  error double precision is not supported\n"
    "#endif\n"
    "kernel void run(\n"
    "       ulong n,\n"
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

    "    if (i < n) {\n"
    "       out[i] = ";

static char cmd[65536];

static const char *outro  =
    ";\n    }\n"
    "}\n";

int main(int argc, char** argv) {

    if (argc < 2) {
	std::cout << "Sintassi: LogTab <input_file.txt>" << std::endl;
	return 1;
    }

    const size_t N = 1 << 20;

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
	char* p = (char*) cmd;
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

		else {
			*p++ = c;
		}
	}

	fclose(fi);
 
	char *source = (char*) malloc(sizeof(char)*(strlen(intro)+strlen(cmd)+strlen(outro)));
	strcpy(source, intro);
	strcpy(source+strlen(intro), cmd);
	strcpy(source+strlen(intro)+strlen(cmd), outro);

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

	std::vector<short> out(N);

	cl::Buffer OUT(context, CL_MEM_READ_WRITE,
		out.size() * sizeof(short));

	// Set kernel parameters.
	run.setArg(0, static_cast<cl_ulong>(N));
	run.setArg(1, OUT);

	std::cout << device[0].getInfo<CL_DEVICE_NAME>() << ": Pronto!\nComando: " << cmd << std::endl;
	
	// Launch kernel on the compute device.
	queue.enqueueNDRangeKernel(run, cl::NullRange, N, cl::NullRange);

	// Get result back to host.
	queue.enqueueReadBuffer(OUT, CL_TRUE, 0, out.size() * sizeof(short), out.data());

	bool fal = false;
	bool tru = false;
	for (int i = 0; i < (1 << 6); ++i) {
		if (out[i]) tru = true;
		if (!out[i]) fal = true;
	}

	if (tru && fal)
		std::cout << "Risultato: Opinione" << std::endl;
	else if (tru)
		std::cout << "Risultato: Tautologia" << std::endl;
	else
		std::cout << "Risultato: Paradosso" << std::endl;

    } catch (const cl::Error &err) {
	std::cerr
	    << "Errore OpenCL: "
	    << err.what() << "(" << err.err() << ")"
	    << std::endl;
	return 1;
    }

    return 0;
}
