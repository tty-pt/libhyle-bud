/**
 * Binds schema field definitions to table rows for UI rendering
 */
export function bindSchemaTable(schemaFields, rows) {
  const headers = schemaFields.map((f) => ({
    key: f.name || f.key,
    label: f.label || f.name || f.key
  }));

  const formattedRows = rows.map((row) => {
    const cells = {};
    for (const h of headers) {
      cells[h.key] = row[h.key] !== undefined && row[h.key] !== null ? String(row[h.key]) : "";
    }
    return cells;
  });

  return { headers, rows: formattedRows };
}

export function version() {
  return "0.1.0";
}
