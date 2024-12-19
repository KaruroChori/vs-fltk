//Secrets are similar to elements in permanent storage, but they are used for autofilling purposes and to store credentials which can be manually or automatically used.
import { sqliteTable, integer, uniqueIndex, text, blob } from "drizzle-orm/sqlite-core"
import { apps } from "./apps";

//Secrets table offering a path:value storage solution for persistent data to fill in each app.
export const secrets = sqliteTable('secrets', {
  id: integer().primaryKey(),
  app: integer().references(()=> apps.id, {onDelete:'cascade',onUpdate:'cascade'}).notNull(),
  //Multiple variants of autofilling are possible, use tag to pick one. If unique it is tagged as NULL
  tag: text(),
  path: text().notNull(),
  value: blob().notNull(),
},(table)=>({idx:uniqueIndex('secretsUniqueIdx').on(table.app,table.tag,table.path)}));