> [!IMPORTANT]  
> This project is very much in its infancy.
> These specs will provide information about features which are yet to be implemented.  
> Broadly speaking, they target **at least** `v0.5.x` from the [milestone page](./milestones.md)

# Quick overview

`vs` is a lightweight runtime to design and run custom user interfaces based on [fltk](https://www.fltk.org/).  
It aims to be fast, to fit on systems with limited resources available, and to run on virtually any platform.  
The `vs` runtime ships with several built-in features, tools and libraries:

- A self-hosted workspace/explorer to quickly access local and remote apps, to update `vs`, and to visually customize user settings.
- A self-hosted UI builder/IDE to visually inspect, debug or write custom components and applications.
- C bindings to expose most features and allow custom native components from third parties.
- A language server & extension to directly handle `.vs` and `.tvs` files in vscode and compatible editors.

## Structure of an application

- An entry-point file referred as **app**, whose root element is `app`.
- **Components**, whose root element is `component`, either used from the entry-point file or other components.
- **Data sources**, usually files with a specific format or endpoints with a well-defined interface.
- **Assets** like images.
- An optional **external application** usually as a dynamic library linked with the **app**, or indirectly used via a Unix socket domain.

Furthermore, there are additional elements which are not controlled by those writing the app, but on the user side:

- A storage where secrets and persistent data can be saved.
- A profile with clear permissions and safety policies. An application can provide one as asset suggesting the user to adopt it, but it has no control over what the user decides.

Both **apps** and **components** can be either specified as final `vs` xml files, or they can be the result of some static data and a template compiled down into "final" xml files.

# Basic VS elements

### basic attributes for frame-able components

Except for some special entity or those which cannot have a `vs` frame, there are some attributes shared by everyone

| attribute      | type      | default | description |
| -------------- | --------- | ------- | ----------- |
| `name`         | `string?` |         |             |
| `mixins`       |           |         |             |
| `frame.mode`   |           |         |             |
| `frame.access` |           |         |             |
| `frame.type`   |           |         |             |

### basic attributes for fl:xxx

TODO: Add link to automatically generated docs

## app

### attributes

| attribute   | type         | default | description                                                                                          |
| ----------- | ------------ | ------- | ---------------------------------------------------------------------------------------------------- |
| `key`       | `blob(256)?` | `null`  | If set, this is the class key used by the component. It requires _fat_ components                    |
| `tag`       | `string?`    | `null`  | If set, this is the _page-tag_ information to distinguish different pages within the same class keys |
| `link-with` | `path?`      | `null`  | If set, the dynamic library to expose to each embedded script                                        |

## component

### attributes

| attribute    | type         | default | description                                                                                                    |
| ------------ | ------------ | ------- | -------------------------------------------------------------------------------------------------------------- |
| `thin`       | `boolean`    | `true`  | If true, this component will not allow storing app class keys, policies and path information                   |
| `auto-prune` | `boolean`    | `false` | If true, remove this component from the tree, directly expanding its children. Incompatible with `thin: false` |
| `key`        | `blob(256)?` | `null`  | If set, this is the class key used by the component. It requires _fat_ components                              |
| `tag`        | `string?`    | `null`  | If set, this is the _page-tag_ information to distinguish different pages within the same class keys           |

## namespace

## metadata

`metadata` are is a pseudo-element which must be placed as direct children of an `app` or `component` entity.  
Entries inside it will be parsed for documentation, attribution or to provide versioning information.

## import

`import` are directives which must be placed as direct children of an `app` or `component` entity.

## use

## data

### attributes

- `src`
- `filter`
- `sort`
- `order`

## debug

### attributes

- `key`
- `value`

## viewport

### attributes

- `src`

## elements with a frame

#### attributes

- `name`
- `mixins`
- `frame.mode`
- `frame.acces`
- `frame.type`

### script

`script`s are pseudo-entities which must be placed as direct children of a frame-able entity. Only one for each frame-able entity can be defined.

#### attributes

| attribute | type               | default  | description                                                                                 |
| --------- | ------------------ | -------- | ------------------------------------------------------------------------------------------- |
| `lang`    | `string?`          | `null`   | If unset, the language will be automatically inferred from contextual information           |
| `src`     | `path?`            | `null`   | If set, its content will only be used as fall-back and the src content will be preferred.   |
| `type`    | _single_, _module_ | _single_ | Used to specify if it should be handled as a single user script or a shared one (TODO:link) |
| `compact` | `boolean`          | `false`  | If set, all code is assumed to be inside the body of the default callback.                  |

### mixin

`mixin`s are pseudo-entities which must be placed as direct children of a frame-able entity. Only one for each frame-able entity can be defined.

#### attributes

### semantics

# Using components

There are two ways to use other components inside others or within your app:

- Via `vs:use`, suggested for cases where it is only needed once.
- Adding a `vs:import` directive in the app/component, and using it as an element under the specified name.

There is also `vs:viewport` which does something similar, but in this case a new root is rendered within your document, with no event or symbol being passed top/down or bottom/up.

### Examples

```xml
<vs:component tag.suggested="component-a">
    <vs:import src="this://component-b.vs" tag="component-b"/>

    <component-b prop-1="value-1"/>
</vs:component>
```

```xml
<vs:component tag.suggested="component-a">
    <vs:use vs:src="this://component-b.vs" prop-1="value-1"/>
</vs:component>
```

## Names

Names are unique identifiers within a namespace for components.  
Adding a name to a component instance also implies that a vs frame is created for it. The converse is also true, and if no explicit name is assigned, one will be automatically generated.  
Related to names there are two undefined behaviours:

- Giving the same name to two or more elements within the same namespace.
- Accessing elements for which the name has not been manually defined.

### Restrictions on names

At this time, no restriction on names has been defined nor implemented.

## Scope of visibility

The scope within names are visible is called _namespace_. A namespace is not strictly speaking equivalent to the UI tree structure, and all elements within a namespace, regardless of their child/parent ordering, are mutually visible.

### Transparent nodes

Some elements will have their associated frame and name, but their mode is set to `transparent`.  
In this case any child will be part of the same namespace as their parent, ignoring their tree dependency.

### Limitations on visibility

Elements for which their mode is set to `container` or `slot-container` will prevent name resolution upwards from children.  
Elements for which their mode is set to `slot` or `slot-container` will prevent name resolution downwards to its children from nodes above it.  
A node, will always be able to access up and down, regardless of the mode set. Limitations are only for others going through it.

## Props

## Computed values

## Symbols

### Callbacks

### Unknowns

### Fields (Setter/Getters)

### Dispatcher

### Draw

## Event propagation

## Scripts

Embedded scripts are

### Single user scripts

### Module scripts

## Mixins

```xml
<app>
    <mixin name="..." args.../>
    <mixins src=""/>

    <item mixin="class1,class2,class3" overrides.../>
</app>
```

Mixins can be for:

- `*` Everyone
- ~~`ns:*` Everyone in xml namespace ns~~
- `+tag1,+tag2,...` a list of tags
- `class1,class2` elements with that token in the `mixin` attribute

# Topics

- [Templating](templating.md)
- [Data](data.md)
- [Scripting](scripting.md)
- [Virtual paths](paths.md)
- [Settings & user profiles](settings.md)
