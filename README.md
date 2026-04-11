# cexcel

Minimal spreadsheet engine written in C. Operates on plain text files and evaluates simple formulas.

## Input

Text file representing a table:

```
A           | B
1           | 2
3           | 4
=A1+B1      | =A2+B2
            | 1
=A4+B4      | 0
```

* Columns separated by `|`
* Cells can be:

  * numbers
  * empty
  * formulas starting with `=`
* Cell references use `<Column><Row>` (e.g. `A1`, `B2`)

## Output

Evaluated table:

```
A           | B
1           | 2
3           | 4
3           | 7
            | 1
<error>     | 0
```

## Supported

* Integer values
* `+` operator
* Cell references

## Errors

* Invalid references
* Invalid expressions
* Cycles (if implemented)

## Build

Uses `nob.h`.

```
cc nob.c -o nob
./nob
```

## Run

```
./build/excel input.txt
```

## Notes

* No UI
* No persistence
* Full recomputation on each run
* No incremental evaluation
* No dependency graph optimization
