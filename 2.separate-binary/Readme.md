## Separate Binary

This is an example where you can have a separate `.cl` file that you can read and write to. This code, compiles that CL file and writes it to a binary. It then runs the binary on the machine. This is useful when you want to compile your CL code once and distribute it.

Be careful. CL Binaries only apply to the devices and platforms they have been built for. Do not compile a CL binary for a platform on which it cannot run 
