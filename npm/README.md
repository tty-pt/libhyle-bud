# @ttypt/hyle-bud

[![npm version](https://img.shields.io/badge/npm-v0.1.0-CB3837?logo=npm)](#)
[![License: BSD-2-Clause](https://img.shields.io/badge/License-BSD--2--Clause-blue)](#)
[![TypeScript Ready](https://img.shields.io/badge/TypeScript-Ready-3178C6?logo=typescript)](#)
[![Zero Dependencies](https://img.shields.io/badge/dependencies-0-success)](#)

> Universal Node.js and Browser bindings for **`libhyle-bud`**, the schema-to-bud row and filter binding interface.

---

## 1. Quickstart

```bash
npm install @ttypt/hyle-bud
```

```typescript
import { bindSchemaTable } from "@ttypt/hyle-bud";

const schema = [
  { key: "title", label: "Song Title" },
  { key: "year", label: "Year" }
];

const binding = bindSchemaTable(schema, [{ title: "Ave Maria", year: 1985 }]);
console.log(binding.headers);
```

---

## 2. C Development Headers via NPM

```makefile
CFLAGS += -I./node_modules/@ttypt/hyle-bud/include
LDLIBS += -lhyle-bud
```

## License

BSD 2-Clause License. Copyright (c) 2026, tty-pt.
