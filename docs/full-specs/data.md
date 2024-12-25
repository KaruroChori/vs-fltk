# Data

Data elements are the main and most idiomatic mechanism in `vs` to include state in applications.

## General structure

### Attributes

- **readonly** as boolean. Defaults to `false`. If true, no mutation is allowed from within this application.
- **volatile** as boolean. Defaults to `true`. If true it must be expected that events outside the scope of this app can change data.
- **src** as string. The source path of data. It must be specified.
- **schema** as string. The source path to a schema file. Used as metadata for static checks when the associated datasource is used.

## Subtypes

### CSV

#### Additional attributes

- **header** as boolean. Defaults to `false`. If true the first row is used as schema information or skipped if `schema` is already provided.
- **separator.field** as string. Defaults to `\t`. The character to split columns.
- **separator.record** as string. Defaults to `\n`. The character to split records.

### FS

### JSON

### SQLITE

### XML
