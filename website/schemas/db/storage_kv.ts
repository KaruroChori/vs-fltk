import { sqliteTable, integer, uniqueIndex, text, blob } from "drizzle-orm/sqlite-core"
import { apps } from "./apps";

//Simple table offering a key:value storage solution for persistent data of each app.
export const storage_kv = sqliteTable('storage_kv', {
  id: integer().primaryKey(),
  app: integer().references(()=> apps.id, {onDelete:'cascade',onUpdate:'cascade'}).notNull(),
  key: text().notNull(),
  value: blob().notNull(),
  timestamp_creation: integer().notNull(),
  timestamp_expiring: integer().notNull(), //If null no expiration
},(table)=>({idx:uniqueIndex('kvUniqueIdx').on(table.app,table.key)}));