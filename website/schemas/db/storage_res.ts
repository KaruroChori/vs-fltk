import { sqliteTable, integer, blob, text, uniqueIndex } from "drizzle-orm/sqlite-core"
import { apps } from "./apps";

//Persistent cache of remote resources, to avoid delays in loading
export const storage_res = sqliteTable('storage_res', {
  id: integer().primaryKey({autoIncrement:true}),
  app: integer().references(()=> apps.id, {onDelete:'cascade',onUpdate:'cascade'}).notNull(),
  src: text().notNull(),      //URL of the resource
  content: blob().notNull(),
  hash: blob().notNull(),     //Hash to verify the fresheness of this resource.
  timestamp_creation: integer().notNull(),
  timestamp_expiring: integer(), //If null no expiration
},(table)=>({idx:uniqueIndex('resUniqueIdx').on(table.app,table.src)}));