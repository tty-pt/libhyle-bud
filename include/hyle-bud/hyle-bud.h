#ifndef HYLE_BUD_H
#define HYLE_BUD_H

/**
 * @file hyle-bud.h
 * @brief Bridge from hyle row data to bud tree rendering.
 *
 * The only bud-dependent hyle bridge: binds JSON/state row values onto
 * bud nodes and schema descriptors for SSR and WASM alike.
 */

#include <bud/bud.h>
#include <bud/bud_jsx.h>
#include <hyle/field.h>
#include <hyle-source/picker.h>
#include <hyle/schema.h>

/**
 * @brief Bind JSON object values onto a state struct described by a schema.
 *
 * @param[in] state  Target struct to populate.
 * @param[in] fields Zero-terminated schema descriptor array.
 * @param[in] json   JSON object string whose keys match field names.
 */
void hyle_bud_state_apply(
        void *state, const hyle_schema_desc_t *fields, const char *json);

/**
 * @brief Bind JSON object values onto a state struct, with explicit length.
 *
 * @param[in] state  Target struct to populate.
 * @param[in] fields Zero-terminated schema descriptor array.
 * @param[in] json   JSON object string whose keys match field names.
 * @param[in] len    Length of @p json in bytes.
 */
void hyle_bud_state_apply_len(
        void *state, const hyle_schema_desc_t *fields, const char *json,
        size_t len);

/** @brief Option row: an id/label pair. Alias for hyle_option_t. */
typedef hyle_option_t hyle_bud_option_t;
/** @brief Picker widget descriptor. Alias for hyle_picker_desc_t. */
typedef hyle_picker_desc_t hyle_bud_picker_desc_t;
/** @brief One picker field's view entry. Alias for hyle_picker_entry_t. */
typedef hyle_picker_entry_t hyle_bud_picker_entry_t;
/** @brief Collection of picker field entries. Alias for hyle_picker_view_t. */
typedef hyle_picker_view_t hyle_bud_picker_view_t;
/** @brief Storage buffers backing a picker view. Alias for hyle_picker_buffer_t. */
typedef hyle_picker_buffer_t hyle_bud_picker_buffer_t;

/** @brief Maximum number of option rows per picker page. */
#define HYLE_BUD_PICKER_MAX_OPTS HYLE_PICKER_MAX_OPTS
/** @brief Maximum number of pinned selections per picker. */
#define HYLE_BUD_PICKER_MAX_SEL HYLE_PICKER_MAX_SEL
/** @brief Maximum number of picker fields in a view. */
#define HYLE_BUD_PICKER_MAX_FIELDS HYLE_PICKER_MAX_FIELDS
/** @brief Query-string budget for picker state. */
#define HYLE_BUD_PICK_QS_BUDGET HYLE_PICKER_QS_BUDGET

/** @brief Forward declaration of json-c's object type. */
struct json_object;

/**
 * @brief Parse picker JSON state into a view and its storage buffer.
 *
 * @param[in]  json    JSON state string to parse.
 * @param[in]  jlen    Length of @p json in bytes.
 * @param[in]  key     Form field name.
 * @param[in]  target  Dataset the picker selects from.
 * @param[in]  multi   Non-zero for multi-select, zero for single.
 * @param[in]  q       Current search text.
 * @param[in]  page    Current page number.
 * @param[out] buf     Storage buffers for the parsed options.
 * @param[out] pv_out  Picker view filled with the JSON contents.
 */
void hyle_bud_picker_state_from_json(
        const char *json, size_t jlen, const char *key, const char *target,
        int multi, const char *q, int page,
        hyle_bud_picker_buffer_t *buf, hyle_bud_picker_view_t *pv_out);

/**
 * @brief Write a picker view's options and selections into a json-c object.
 *
 * @param[in] pv     Picker view to serialize.
 * @param[in] j_root json-c object to add the pick_opts/pick_sel keys to.
 */
void hyle_bud_picker_state_to_json(
        const hyle_bud_picker_view_t *pv, struct json_object *j_root);

/**
 * @brief Translation callback type.
 *
 * @param[in] msgid Message identifier to translate.
 * @return Translated string.
 */
typedef const char *(*hyle_bud_translate_fn)(const char *msgid);

/**
 * @brief Install the string translator used for UI labels.
 *
 * @param[in] fn Translator callback, or NULL to clear.
 */
void hyle_bud_set_translator(hyle_bud_translate_fn fn);

/**
 * @brief Translate a message identifier via the installed translator.
 *
 * @param[in] msgid Message identifier.
 * @return Translated string, or @p msgid when no translator is set.
 */
const char *hyle_bud_tr(const char *msgid);

/**
 * @brief Build a plain text filter input.
 *
 * @param[in] key           Form field name.
 * @param[in] label         Display label and placeholder.
 * @param[in] current_value Current value to prefill.
 * @return Text input bud node.
 */
bud_node *hyle_bud_text_input(
	const char *key,
	const char *label,
	const char *current_value);

/**
 * @brief Filter widget for a field of a given type.
 *
 * Renders a boolean checkbox, multiselect, dropdown, or text input
 * depending on the field type and the filter style.
 *
 * @param[in] key           Form field name.
 * @param[in] label         Display label.
 * @param[in] type          Hyle field type constant.
 * @param[in] current_value Current value to prefill.
 * @param[in] options       Option rows for reference fields.
 * @param[in] noptions      Number of entries in @p options.
 * @param[in] filter_style  e.g. "dropdown", "multiselect", "grid".
 * @return Filter bud node.
 */
bud_node *hyle_bud_filter_field(
	const char *key,
	const char *label,
	int type,
	const char *current_value,
	const hyle_bud_option_t *options,
	int noptions,
	const char *filter_style);

/*
 * Schema-Driven Generic Form Builder (Entity Pattern)
 */
/**
 * @brief Schema-driven generic form for an entity record.
 *
 * @param[in] schema       Zero-terminated schema descriptor array.
 * @param[in] record       C struct to read current values from.
 * @param[in] action       Form action URL.
 * @param[in] cancel_href  Cancel link URL, or NULL to omit.
 * @param[in] submit_label Submit button text.
 * @param[in] csrf_token   CSRF token for a hidden field, or NULL.
 * @param[in] pv           Picker view for reference fields.
 * @param[in] vstr_val     Value for variable-length string fields.
 * @return Form bud node.
 */
bud_node *hyle_bud_form(
        const hyle_schema_desc_t *schema,
        const void *record,
        const char *action,
        const char *cancel_href,
        const char *submit_label,
        const char *csrf_token,
        const hyle_bud_picker_view_t *pv,
        const char *vstr_val);

/*
 * Schema-Driven Generic Filter Component (Entity Pattern)
 */
/**
 * @brief Schema-driven generic filter component for one field.
 *
 * Automatically draws the matching UI component (search input, boolean
 * checkbox, single-reference dropdown, multi-reference facet dropdown, or
 * omnisearch picker) from the field's entry in a schema descriptor.
 *
 * @param[in] desc          Zero-terminated schema descriptor array.
 * @param[in] field_name    Field whose descriptor drives the widget.
 * @param[in] current_value Current value to prefill.
 * @param[in] pv            Picker view for reference fields.
 * @return Filter bud node.
 */
bud_node *hyle_bud_filter(
	const hyle_schema_desc_t *desc,
	const char *field_name,
	const char *current_value,
	const hyle_bud_picker_view_t *pv);

/**
 * @brief Schema-driven filter built from explicit option rows.
 *
 * @param[in] desc          Zero-terminated schema descriptor array.
 * @param[in] field_name    Field whose descriptor drives the widget.
 * @param[in] current_value Current value to prefill.
 * @param[in] options       Option rows for reference fields.
 * @param[in] noptions      Number of entries in @p options.
 * @return Filter bud node.
 */
bud_node *hyle_bud_filter_from_schema(
	const hyle_schema_desc_t *desc,
	const char *field_name,
	const char *current_value,
	const hyle_bud_option_t *options,
	int noptions);

/**
 * @brief Scoped/indexed filter for repeated rows, tables or cards.
 *
 * A scope of 0 turns "field" into field_0, pickq-field_0, pick_q_field_0.
 *
 * @param[in]  desc               Zero-terminated schema descriptor array.
 * @param[in]  field_name         Field to build a filter for.
 * @param[in]  scope              Row index, or -1 for unscoped.
 * @param[in]  current_value      Current value to prefill.
 * @param[in]  current_label      Label for the current selection.
 * @param[in]  get_action         GET action for the sibling form.
 * @param[in]  pv                 Picker view for reference fields.
 * @param[in]  is_active          Whether this scope is currently active.
 * @param[in]  extra_class        Extra CSS class, or NULL.
 * @param[out] sibling_forms_out  Receives sibling GET forms, or NULL.
 * @return Filter bud node.
 */
bud_node *hyle_bud_filter_scoped(
	const hyle_schema_desc_t *desc,
	const char *field_name,
	int scope,
	const char *current_value,
	const char *current_label,
	const char *get_action,
	const hyle_bud_picker_view_t *pv,
	int is_active,
	const char *extra_class,
	bud_node **sibling_forms_out);

/**
 * @brief Generic filter group / bar builder from a schema and field list.
 *
 * @param[in] desc        Zero-terminated schema descriptor array.
 * @param[in] field_names Array of field names to include.
 * @param[in] n_fields    Number of names in @p field_names.
 * @param[in] current_qs  Query string to read current values from.
 * @param[in] pv          Picker view for reference fields.
 * @return Fragment containing the filter widgets.
 */
bud_node *hyle_bud_filter_group(
	const hyle_schema_desc_t *desc,
	const char **field_names,
	int n_fields,
	const char *current_qs,
	const hyle_bud_picker_view_t *pv);

/**
 * @brief Reset multi-select widget state.
 *
 * Currently a no-op hook kept for symmetry between SSR and WASM builds.
 */
void hyle_bud_ms_reset(void);

/**
 * @brief Multi-select dropdown widget (SSR-first, WASM-enhanced).
 *
 * filter_style "dropdown" selects it for HYLE_FIELD_MULTI_REFERENCE fields.
 *
 * @param[in] key           Form field name.
 * @param[in] label         Display label.
 * @param[in] current_value Comma-separated current selections.
 * @param[in] options       Option rows to choose from.
 * @param[in] noptions      Number of entries in @p options.
 * @return Multi-select bud node, or NULL without options.
 */
bud_node *hyle_bud_multiselect_field(
	const char *key,
	const char *label,
	const char *current_value,
	const hyle_bud_option_t *options,
	int noptions);

/**
 * @brief Dropdown single-select widget (SSR-first, WASM-enhanced).
 *
 * filter_style "dropdown" selects it for HYLE_FIELD_REFERENCE fields.
 *
 * @param[in] key           Form field name.
 * @param[in] label         Display label.
 * @param[in] current_value Current selection id.
 * @param[in] options       Option rows to choose from.
 * @param[in] noptions      Number of entries in @p options.
 * @return Dropdown bud node, or NULL without options.
 */
bud_node *hyle_bud_reference_select_dropdown(
	const char *key,
	const char *label,
	const char *current_value,
	const hyle_bud_option_t *options,
	int noptions);

/**
 * @brief Build a sortable table header row.
 *
 * @param[in] col_keys   Column keys used for sort URLs.
 * @param[in] col_labels Column display labels.
 * @param[in] ncols      Number of columns.
 * @param[in] sort_field Currently sorted column key, or NULL.
 * @param[in] sort_asc   Non-zero for ascending sort.
 * @param[in] qs         Existing query string to preserve.
 * @return thead bud node.
 */
bud_node *hyle_bud_table_header(
	const char **col_keys,
	const char **col_labels,
	int ncols,
	const char *sort_field,
	int sort_asc,
	const char *qs);

/**
 * @brief Build a table body from row ids and values.
 *
 * @param[in] col_keys   Column keys.
 * @param[in] col_labels Column display labels.
 * @param[in] ncols      Number of columns.
 * @param[in] ids        Row ids; the first cell links to /{module}/{id}.
 * @param[in] nids       Number of rows.
 * @param[in] values     Flat nids*ncols array of cell strings.
 * @param[in] module     Module name for row link URLs.
 * @return tbody bud node.
 */
bud_node *hyle_bud_table_body(
	const char **col_keys,
	const char **col_labels,
	int ncols,
	const char **ids,
	int nids,
	const char **values,
	const char *module);

/**
 * @brief Build a full sortable table.
 *
 * @param[in] col_keys   Column keys used for sort URLs.
 * @param[in] col_labels Column display labels.
 * @param[in] ncols      Number of columns.
 * @param[in] ids        Row ids.
 * @param[in] nids       Number of rows.
 * @param[in] values     Flat nids*ncols array of cell strings.
 * @param[in] module     Module name for row link URLs.
 * @param[in] sort_field Currently sorted column key, or NULL.
 * @param[in] sort_asc   Non-zero for ascending sort.
 * @param[in] qs         Existing query string to preserve.
 * @return Table bud node.
 */
bud_node *hyle_bud_table(
	const char **col_keys,
	const char **col_labels,
	int ncols,
	const char **ids,
	int nids,
	const char **values,
	const char *module,
	const char *sort_field,
	int sort_asc,
	const char *qs);

/**
 * @brief Build a pagination bar for a result table.
 *
 * @param[in] page      Current page number (1-based).
 * @param[in] per_page  Rows per page.
 * @param[in] total     Total row count.
 * @param[in] row_count Rows on the current page.
 * @param[in] qs        Existing query string (reserved).
 * @return Pagination bud node.
 */
bud_node *hyle_bud_pagination(
	int page,
	int per_page,
	int total,
	int row_count,
	const char *qs);

/**
 * @brief Row-action descriptor for hyle_bud_table_actions.
 *
 * Makes each row uniformly activable via a stretched overlay element in the
 * first cell. LINK renders an <a> (href defaults to /{module}/{id}); SUBMIT
 * renders a <button type=submit> that posts to the form named by form_id via
 * the HTML5 form= attribute (no-JS friendly, works inside another enclosing
 * form). css_class lets an <a> look like a button.
 */
typedef enum {
	/** No action; the row is not activable. */
	HYLE_ROW_ACTION_NONE = 0,
	/** Row action is a link, defaulting to /{module}/{id}. */
	HYLE_ROW_ACTION_LINK,
	/** Row action is a submit button posting to the named form. */
	HYLE_ROW_ACTION_SUBMIT
} hyle_row_action_kind_t;

/**
 * @brief Per-row action descriptor for hyle_bud_table_actions.
 */
typedef struct {
	/** @brief Action kind (hyle_row_action_kind_t). */
	int kind;
	/** @brief Extra CSS class, e.g. "btn". */
	const char *css_class;
	/** @brief Visible text; ""/NULL = pure overlay. */
	const char *label;
	/** @brief aria-label prefix, e.g. "Add"/"Open". */
	const char *aria_base;
	/** @brief LINK: overrides the "/{module}/" default href. */
	const char *href_base;
	/** @brief SUBMIT: target form's id attribute. */
	const char *form_id;
	/** @brief SUBMIT: field name (e.g. "song_id"); value = row id. */
	const char *field_name;
} hyle_bud_row_action_t;

/**
 * @brief Build a table whose rows carry a uniform action overlay.
 *
 * @param[in] col_keys   Column keys used for sort URLs.
 * @param[in] col_labels Column display labels.
 * @param[in] ncols      Number of columns.
 * @param[in] ids        Row ids.
 * @param[in] nids       Number of rows.
 * @param[in] values     Flat nids*ncols array of cell strings.
 * @param[in] module     Module name for row link URLs.
 * @param[in] sort_field Currently sorted column key, or NULL.
 * @param[in] sort_asc   Non-zero for ascending sort.
 * @param[in] qs         Existing query string to preserve.
 * @param[in] action     Row action to apply to every row.
 * @return Table bud node.
 */
bud_node *hyle_bud_table_actions(
	const char **col_keys,
	const char **col_labels,
	int ncols,
	const char **ids,
	int nids,
	const char **values,
	const char *module,
	const char *sort_field,
	int sort_asc,
	const char *qs,
	const hyle_bud_row_action_t *action);

/**
 * @brief Full picker widget: trigger summary plus options panel.
 *
 * @param[in] d Picker descriptor.
 * @return Picker bud node, or NULL for an unusable descriptor.
 */
bud_node *hyle_bud_picker_field(const hyle_bud_picker_desc_t *d);

/*
 * Standalone / Action Picker Component
 */
/**
 * @brief Spec for hyle_bud_action_picker.
 *
 * Encapsulates the picker field, the No-JS sibling GET form, preference
 * preservation, auto-submit attributes, and the enclosing POST form.
 */
typedef struct {
	/** @brief Form field key for the picker selection. */
	const char *key;
	/** @brief Picker trigger label. */
	const char *label;
	/** @brief Dataset the picker selects from. */
	const char *target;
	/** @brief Default selection id when no view entry exists. */
	const char *default_id;
	/** @brief Label for the default selection. */
	const char *default_label;
	/** @brief GET action for the no-JS sibling search form. */
	const char *get_action;
	/** @brief POST action for the enclosing submit form. */
	const char *post_action;
	/** @brief id attribute of the enclosing POST form. */
	const char *form_id;
	/** @brief CSRF token for a hidden field in the POST form. */
	const char *csrf_token;
	/** @brief Text of the POST form's submit button. */
	const char *submit_label;
	/** @brief Optional header text above the widget. */
	const char *header_text;
	/** @brief Cancel link URL, or NULL to omit. */
	const char *cancel_href;
	/** @brief Text of the cancel link. */
	const char *cancel_label;
	/** @brief Hint text below the widget. */
	const char *hint;
	/** @brief Optional scope suffix for scoped param names. */
	const char *scope;
	/** @brief Custom search input name, or NULL for the default. */
	const char *search_param;
	/** @brief Custom page input name, or NULL for the default. */
	const char *page_param;
	/** @brief Non-zero to emit an auto-submit enhancement attribute. */
	int auto_submit;
	/** @brief Non-zero to allow inline record creation. */
	int allow_add;
	/** @brief Preference names preserved as hidden inputs. */
	const char **pref_names;
	/** @brief Values matching pref_names. */
	const int *pref_vals;
	/** @brief Number of preferences in pref_names/pref_vals. */
	int n_prefs;
	/** @brief Extra hidden inputs appended to the POST form. */
	bud_node *extra_post_inputs;
} hyle_bud_action_picker_spec_t;

/**
 * @brief Standalone action picker: field, sibling GET form and POST form.
 *
 * @param[in] spec Spec describing the widget.
 * @param[in] pv   Picker view to render state from.
 * @return Fragment bud node, or NULL for invalid specs.
 */
bud_node *hyle_bud_action_picker(
        const hyle_bud_action_picker_spec_t *spec,
        const hyle_bud_picker_view_t *pv);

/**
 * @brief Render panel innards and summary span HTML for the reset path.
 *
 * Caller owns both buffers.
 *
 * @param[in]  d         Picker descriptor.
 * @param[out] panel     Buffer for the panel HTML.
 * @param[in]  panel_sz  Size of @p panel.
 * @param[out] values    Buffer for the summary span HTML.
 * @param[in]  values_sz Size of @p values.
 */
void hyle_bud_picker_slots(const hyle_bud_picker_desc_t *d,
        char *panel, size_t panel_sz, char *values, size_t values_sz);

/**
 * @brief Render only the option-row chunk (infinite-scroll append path).
 *
 * Renders the desc's current page; caller owns the buffer.
 *
 * @param[in]  d       Picker descriptor whose current page to render.
 * @param[out] rows    Buffer for the rows HTML.
 * @param[in]  rows_sz Size of @p rows.
 */
void hyle_bud_picker_rows(const hyle_bud_picker_desc_t *d,
        char *rows, size_t rows_sz);

/*
 * Active picker scope discovery helper
 */
/**
 * @brief Discover the active picker scope from a query string.
 *
 * @param[in]  qs        Query string to scan.
 * @param[out] scope_buf Buffer for the scope string.
 * @param[in]  scope_sz  Size of @p scope_buf.
 * @return Scope index, or -1 when none is active.
 */
int hyle_bud_pick_find_active_scope(const char *qs, char *scope_buf, size_t scope_sz);

/*
 * Schema-Driven Picker View Collection (Entity Pattern)
 */
/**
 * @brief Collect picker entries for all reference fields in a schema.
 *
 * @param[in]  qs               Query string containing picker state.
 * @param[in]  schema           Zero-terminated schema descriptor array.
 * @param[in]  record           Record to read current values from.
 * @param[out] pv_out           Picker view to fill.
 * @param[out] active_scope_out Receives the active scope, or NULL.
 * @return Number of entries collected.
 */
int hyle_bud_picker_view_collect_schema(
        const char *qs,
        const hyle_schema_desc_t *schema,
        const void *record,
        hyle_bud_picker_view_t *pv_out,
        int *active_scope_out);

/**
 * @brief Collect picker entries with an explicit scope suffix.
 *
 * @param[in]  qs     Query string containing picker state.
 * @param[in]  schema Zero-terminated schema descriptor array.
 * @param[in]  record Record to read current values from.
 * @param[out] pv_out Picker view to fill.
 * @param[in]  scope  Scope suffix, or NULL/empty for unscoped.
 * @return Number of entries collected.
 */
int hyle_bud_picker_view_collect_scoped(
        const char *qs,
        const hyle_schema_desc_t *schema,
        const void *record,
        hyle_bud_picker_view_t *pv_out,
        const char *scope);

/**
 * @brief Collect the picker entry matching the query string's active field.
 *
 * @param[in]  qs                   Query string to scan.
 * @param[in]  schema               Zero-terminated schema descriptor array.
 * @param[out] pv_out               Picker view to fill.
 * @param[out] active_field_idx_out Receives the matching field index, or NULL.
 * @param[out] active_scope_out     Receives the matching scope, or NULL.
 * @return Number of entries collected.
 */
int hyle_bud_picker_view_collect_auto_fields_schema(
        const char *qs,
        const hyle_schema_desc_t *schema,
        hyle_bud_picker_view_t *pv_out,
        int *active_field_idx_out,
        int *active_scope_out);

/*
 * C-Struct to JSON State Overlays for WASM Hydration
 */
/**
 * @brief Add a C state struct's fields to a json-c object.
 *
 * @param[in] jo       json-c object to populate.
 * @param[in] state    C struct to read fields from.
 * @param[in] fields   Zero-terminated bud field descriptor array.
 * @param[in] int_kind Field kind treated as int.
 * @param[in] str_kind Field kind treated as string.
 * @return 0 on success, -1 on NULL arguments.
 */
int hyle_bud_state_overlay_from_desc(
        struct json_object *jo,
        const void *state,
        const bud_field_desc_t *fields,
        int int_kind,
        int str_kind);

/**
 * @brief Build a JSON array of objects from an array of C structs.
 *
 * @param[in] items     Array of C structs.
 * @param[in] count     Number of elements.
 * @param[in] elem_size Byte size of one element.
 * @param[in] fields    Zero-terminated bud field descriptor array.
 * @param[in] int_kind  Field kind treated as int.
 * @param[in] str_kind  Field kind treated as string.
 * @return json-c array object, or NULL on allocation failure.
 */
struct json_object *hyle_bud_state_overlay_array(
        const void *items,
        int count,
        size_t elem_size,
        const bud_field_desc_t *fields,
        int int_kind,
        int str_kind);

#endif
