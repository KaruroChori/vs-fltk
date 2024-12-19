import { sqliteTable, integer, blob, text } from "drizzle-orm/sqlite-core"

export const apps = sqliteTable('apps', {
  id: integer().primaryKey({autoIncrement:true}),
  //The 256bit key used to uniquely identify a certain application class. 0 is left for the secret key of the current user.
  app_id: blob({mode:'buffer'}).unique().notNull(),
  //Comma separate list of base paths patterns to be considered part of the namespace of this key
  base: text().notNull(),
  //Additional notes left by the user
  notes: text(),
  //URL of where this application class is defined, or who has authoritative control over it.
  source: text(),
});