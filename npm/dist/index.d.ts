export interface TableBinding {
  headers: Array<{ key: string; label: string }>;
  rows: Array<Record<string, string>>;
}

export declare function bindSchemaTable(
  schemaFields: Array<{ name?: string; key?: string; label?: string }>,
  rows: Array<Record<string, any>>
): TableBinding;

export declare function version(): string;
