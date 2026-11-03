# @tty-pt/hyle-bud

[![npm version](https://img.shields.io/badge/npm-v0.1.0-CB3837?logo=npm)](#)
[![License: BSD-2-Clause](https://img.shields.io/badge/License-BSD--2--Clause-blue)](#)
[![TypeScript Ready](https://img.shields.io/badge/TypeScript-Ready-3178C6?logo=typescript)](#)
[![Zero Dependencies](https://img.shields.io/badge/dependencies-0-success)](#)

> Universal Node.js and Browser bindings for **`libhyle-bud`**, the schema-to-bud row and filter binding interface.

---

## 1. Quickstart

```bash
npm install @tty-pt/hyle-bud
```

```typescript
import { bindSchemaTable } from "@tty-pt/hyle-bud";

const schema = [
  { key: "title", label: "Song Title" },
  { key: "year", label: "Year" }
];

const binding = bindSchemaTable(schema, [{ title: "Ave Maria", year: 1985 }]);
console.log(binding.headers);
```

---

## 2. JavaScript / TypeScript API Reference

### `bindSchemaTable(schemaFields: FieldDescriptor[], rows: Record<string, any>[]): TableBinding`
Binds canonical Hyle schema definitions (`hyle_schema_desc_t`) into tabular presentation data suitable for Bud table rendering.
* `schemaFields`: Array of schema field definitions with `key`/`name` and `label`.
* `rows`: Raw array of record objects.
* Returns `{ headers: Array<{ key: string, label: string }>, rows: Array<Record<string, string>> }`.

### `version(): string`
Returns the library semantic version string.

---

## 3. C Development Headers via NPM

```makefile
CFLAGS += -I./node_modules/@tty-pt/hyle-bud/include
LDLIBS += -lhyle-bud
```

## License

BSD 2-Clause License. Copyright (c) 2026, tty-pt.
