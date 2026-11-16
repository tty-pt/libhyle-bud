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

export function formatInput(name, label, val = "", type = "text", className = "hyle-input-group") {
  const lbl = label || name;
  return {
    name,
    label: lbl,
    value: String(val ?? ""),
    type,
    className
  };
}

export function formatField(field, val = "", className = "hyle-input-group") {
  const fName = field.name || field.key;
  let type = "text";
  if (field.type === "text" || field.file) type = "textarea";
  else if (field.is_int || field.type === "int") type = "number";
  else if (field.type === "bool") type = "checkbox";
  return formatInput(fName, field.label, val, type, className);
}

export function version() {
  return "0.1.0";
}
