# CSV Converter

Some experiments with a CSV->JSON/YAML/whatever converter to explore C++.

## TEST

    ./csv_convert_test
    
## Run on small sample set.

    $ ./csv_convert -c tst.csv  -o tst.json  -f first_field:string -f second_field:string -f third_field:int -f fourth_field:double

    $ cat tst.json
    

## BUGS

1. Emitted JSON strings are not escaped.  
If a CSV string field contains quotes or other JSON-special
characters, the output will not be JSON-compliant.


## IMPROVEMENTS

1. `Record` data is disconnected from their `Specification` counterpart  
   We need a way so that each field in a record directly can access
   its specificaiton field name and type. Preferrably without adding 8
   bytes of specification reference to each field.

2. Speed  
   Using vanilla C++ string and file processing is extremely slow.  
   Use multiple threads to parse different parts of the CSV file simultaneously.  
   Use mmap() to avoid file operations.

3. Create a wrapper for the full transform cycle__
   Today the streaming conversion from an ingestion to an emitter
   is done in a small `while()` loop in the main program. This should
   be encapsulated in a (new?) class to ensure correct behavior.
   
4. Replace dynamic allocation of `Record` in `IngestionIface::ingest_record()`  
   The implementation of `IngestionIface::ingest_record()` needs to create
   a shared object for each record it processes. This is inefficient and should
   be replaced with a caller-provided object reference (from the stack) that the
   `ingest_record()` fills out.
