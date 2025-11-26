
conan install . --output-folder=x64\.conan\Release\
conan install . --output-folder=x64\.conan\Debug\ -s build_type=Debug -s compiler.runtime_type=Debug --build=missing

pause