/*

 Tabelle di verita' su openCL

 Compilazione:

	 c++ -O2 LogTab.cpp -o LogTab.out -lOpenCL

 File di input:

	6 variabili da a[i] a f[i]

	Es:  a[i] && !a[i];

	Es:  (!a[i] || (a[i] && b[i])) || !b[i] || (b[i] && a[i]);
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
    "       global short *out,\n"
    "       global const short *a,\n"
    "       global const short *b,\n"
    "       global const short *c,\n"
    "       global const short *d,\n"
    "       global const short *e,\n"
    "       global const short *f\n"
    "       )\n"
    "{\n"
    "    size_t i = get_global_id(0);\n"
    "    if (i < n) {\n"
    "       out[i] = ";

static char cmd[65536];

static const char *outro  =
    "\n    }\n"
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

    	fread (cmd, 1, 65536, fi);

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

	// Prepare input data.
	std::vector<short> a(N);
	std::vector<short> b(N);
	std::vector<short> c(N);
	std::vector<short> d(N);
	std::vector<short> e(N);
	std::vector<short> f(N);
	std::vector<short> out(N);

	for (int i = 0; i < (1 << 6); ++i) {
		a[i]=!(i&1);
		b[i]=!(i&2);
		c[i]=!(i&4);
		d[i]=!(i&8);
		e[i]=!(i&16);
		f[i]=!(i&32);
	}

	// Allocate device buffers and transfer input data to device.
	cl::Buffer A(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		a.size() * sizeof(short), a.data());

	cl::Buffer B(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		b.size() * sizeof(short), b.data());

	cl::Buffer C(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		c.size() * sizeof(short), c.data());

	cl::Buffer D(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		d.size() * sizeof(short), d.data());

	cl::Buffer E(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		e.size() * sizeof(short), e.data());

	cl::Buffer F(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		f.size() * sizeof(short), f.data());

	cl::Buffer OUT(context, CL_MEM_READ_WRITE,
		out.size() * sizeof(short));

	// Set kernel parameters.
	run.setArg(0, static_cast<cl_ulong>(N));
	run.setArg(1, OUT);
	run.setArg(2, A);
	run.setArg(3, B);
	run.setArg(4, C);
	run.setArg(5, D);
	run.setArg(6, E);
	run.setArg(7, F);

	std::cout << device[0].getInfo<CL_DEVICE_NAME>() << ": Pronto!" << std::endl;
	
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
