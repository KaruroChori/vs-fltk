//It is possible to share secrets defined for one app with a different one. For example an application can log in an other.
import { sqliteTable, integer, uniqueIndex, text, blob } from "drizzle-orm/sqlite-core"
import { apps } from "./apps";

//Secrets table offering a path:value storage solution for persistent data to fill in each app.
export const secrets_shared = sqliteTable('secrets_shared', {
  id: integer().primaryKey(),
  from_app: integer().references(()=> apps.id, {onDelete:'cascade',onUpdate:'cascade'}).notNull(),
  with_app: integer().references(()=> apps.id, {onDelete:'cascade',onUpdate:'cascade'}).notNull(),
  tag: text(),
  notes: text(),
  timestamp_creation: integer({ mode: 'timestamp_ms' }).notNull(),
  timestamp_expiring: integer({ mode: 'timestamp_ms' }).notNull(),
},(table)=>({idx:uniqueIndex('secretsUnique').on(table.from_app,table.with_app,table.tag)}));