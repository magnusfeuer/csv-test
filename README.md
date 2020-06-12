# csv-test
Some experiments with CSV files.

## TEST

    ./csv_convert_test
    
## Running

    ./csv_convert -c <csv-file> -t <type> -o <output-file> -ffield_name:field_type [-f ...]
      -c <csv-file>             CSV file to ingest.
      -T <type>                 CSV Reader type. Default 'csv'
      -t <type>                 Output file type. Default 'json'
      -o <output-file>          Output file name.
      -e <escape-char>          Escape character to use. Default [none].
      -s <separator-char>       Separator character to use. Default ','.
      -f field_name:field_type  CSV field specification.

`field_name` is the name of the given field.
`field_type` is data type. Supported values are `int`, `double`, and `string`.

Supported input types are `csv`.  
Supported output types are `json`.


## BUGS

1. Emitted JSON strings are not escaped.  
If a CSV string field contains quotes or other JSON-special
characters, the output not be JSON-compliant.


## IMPROVEMENTS

1. `Record` data is disconnected from their `Specification`  
   We need a way so that each field in a record directly can access
   its specificaiton field name and type. Preferrably without adding 8
   bytes of specification reference to each field.

2. Speed  
   Using vanilla C++ string and file processing is extremely slow.  
   Use multiple threads to parse different parts of the CSV file simultaneously.  
   Use mmap() to avoid file operations.

3. `IngestionIface::ingest()` should only be friend-callable from `Dataset::ingest()`  
   Calling `Dataset::ingest()` directly does not reset the target
   dataset.  We can have a workaround by adding a `Dataset::reset()`
   method, called by `IngestionIface` implementations, but that is
   something that the implementer is likely to forget.

4. Replace dynamic allocation of `Record` in `IngestionIface::ingest_record()`  
   The implementation of `IngestionIface::ingest_record()` needs to create
   a shared object for each record it processes. This is inefficient and should
   be replaced with a caller-provided object reference (from the stack) that the
   `ingest_record()` fills out.

       

