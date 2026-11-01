import test from "node:test";
import assert from "node:assert/strict";
import { bindSchemaTable, version } from "../dist/index.js";

test("bindSchemaTable formats schema fields and rows", () => {
  const schema = [
    { key: "title", label: "Song Title" },
    { key: "year", label: "Year" }
  ];
  const rows = [
    { title: "Ave Maria", year: 1985 },
    { title: "A Cabana", year: 1978 }
  ];

  const binding = bindSchemaTable(schema, rows);
  assert.equal(binding.headers.length, 2);
  assert.equal(binding.headers[0].label, "Song Title");
  assert.equal(binding.rows.length, 2);
  assert.equal(binding.rows[0].title, "Ave Maria");
  assert.equal(binding.rows[0].year, "1985");

  assert.ok(version().length > 0);
});
