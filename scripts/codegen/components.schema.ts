import { Type as t } from "@sinclair/typebox"

//New schema to replace the old one.
//Now props, computed, setters and getters are better linked together

export const type_schema = t.Union([
    t.Literal('flag'),
    t.Literal('enum'),
    t.Literal('raw'),
    t.Literal('path'),
    t.Literal('string'),
    t.Literal('color'),
    t.Literal('scalar-1'),
    t.Literal('scalar-2'),
    t.Literal('scalar-4')
], { description: 'type', default: 'string' });

const entries_schema = t.Record(t.String(), t.Object({
    type: type_schema,
    subtype: t.Optional(t.String()),
    parse: t.Optional(t.Union([t.String(), t.Null()], { default: undefined, description: "From string to object. Default based on `type` (and `subtype`)" })),
    //`serialize` would be the dual of parse, but it is not needed as types are all defined and serialization methods will not be custom.
    setter: t.Optional(t.Union([t.String(), t.Null()], { default: undefined, description: "Operations on setting. If undefined, this field is only available as a computed value. If null use default" })),
    getter: t.Optional(t.Union([t.String(), t.Null()], { default: undefined, description: "Implementation on how to extract value from object. If undefined the field is not observable. If null use default" })),
    description: t.Optional(t.String()),
    semantic: t.Optional(t.Boolean({ default: false, description: 'If true, this field has a strong semantic meaning. Used for semantic serialization of a document.' })),
    alias: t.Optional(t.Array(t.String(), { description: "alias names", default: [] }))
}, { additionalProperties: false }))

export const widget_schema = t.Object({
    $schema: t.Optional(t.String()),
    ns: t.Optional(t.String({ description: "Namespace for this component, for example `fl` in case of fltk wrappers." })),
    name: t.Optional(t.String()),
    exposed: t.Optional(t.Boolean({ default: true, description: "If true, this widget will be usable from XML and shown in the documentation, otherwise it is not exported." })),
    description: t.Optional(t.String({ description: "While optional, this field is extremely important for exposed widgets" })),
    use_main_header: t.Union([t.Null(), t.String()], { default: null, description: "If set, no class definition will be generated. It will instread load one already present. Used to track and document widgets which are not fully autogenerated." }),
    headers: t.Optional(t.Array(t.String(), { description: "List of header files used by the widget in its public header file." })),
    private_headers: t.Optional(t.Array(t.String(), { description: "List of header files used by the widget in its generated cpp file." })),
    type: t.Union([t.Literal('leaf'), t.Literal('node'), t.Literal('container'), t.Literal('slot'), t.Literal('slot-contaiener')], { description: "Type of frame an instance of this widget class will default top." }),
    codegen: t.Object({
        extends: t.Union([t.Null(), t.String()], { default: null, description: "If set, a class to inherit from, usually related to the fltk widget." }),
        set_tail: t.Optional(t.Union([t.Null(), t.String()], { description: "If specified, what to do if a getter is not matched." })),
        get_tail: t.Optional(t.Union([t.Null(), t.String()], { description: "If specified, what to do if a setter is not matched." })),
    }, { additionalProperties: false }),
    extends: t.Union([t.Null(), t.String()], { default: null, description: "If set, a vs widget to inherit from (not a c++ class name)" }),
    skip_fields: t.Optional(t.Array(t.String(), { default: [], description: "Properties to be matched when set, but ignored." })),
    fields: entries_schema,
}, { additionalProperties: false })