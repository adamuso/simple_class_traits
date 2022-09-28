$ErrorActionPreference = "Stop"

cl /EHsc /std:c11 generic_trait_with_output.cpp /link /out:a.exe && .\a.exe
if ($LASTEXITCODE -ne 0) { throw "Exit code is $LASTEXITCODE" }

cl /EHsc /std:c11 generic_trait.cpp /link /out:a.exe && .\a.exe
if ($LASTEXITCODE -ne 0) { throw "Exit code is $LASTEXITCODE" }

cl /EHsc /std:c11 json_serialize.cpp /link /out:a.exe && .\a.exe
if ($LASTEXITCODE -ne 0) { throw "Exit code is $LASTEXITCODE" }

cl /EHsc /std:c11 macro.cpp /link /out:a.exe && .\a.exe
if ($LASTEXITCODE -ne 0) { throw "Exit code is $LASTEXITCODE" }

cl /EHsc /std:c11 shared_ptr.cpp /link /out:a.exe && .\a.exe
if ($LASTEXITCODE -ne 0) { throw "Exit code is $LASTEXITCODE" }

cl /EHsc /std:c11 simple_print_generic.cpp /link /out:a.exe && .\a.exe
if ($LASTEXITCODE -ne 0) { throw "Exit code is $LASTEXITCODE" }

cl /EHsc /std:c11 simple_print.cpp /link /out:a.exe && .\a.exe
if ($LASTEXITCODE -ne 0) { throw "Exit code is $LASTEXITCODE" }

cl /EHsc /std:c11 simplest_print.cpp /link /out:a.exe && .\a.exe
if ($LASTEXITCODE -ne 0) { throw "Exit code is $LASTEXITCODE" }

cl /EHsc /std:c11 simple_print_generic.cpp /link /out:a.exe && .\a.exe
if ($LASTEXITCODE -ne 0) { throw "Exit code is $LASTEXITCODE" }

cl /EHsc /std:c11 trait_in_vector.cpp /link /out:a.exe && .\a.exe
if ($LASTEXITCODE -ne 0) { throw "Exit code is $LASTEXITCODE" }