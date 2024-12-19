import { sqliteTable, integer, text } from "drizzle-orm/sqlite-core"

/**
 * Bookmarks for vs
 */
export const bookmarks = sqliteTable('bookmarks', {
    id: integer().primaryKey({ autoIncrement: true }),
    //If true it was defined by the user, else inferred by history
    curated: integer({mode:'boolean'}).notNull(),
    //User notes
    notes: text(),
    //List of tags as JSON array
    tags: text({ mode: 'json' }).$type<string[]>(),
    //Recording time
    timestamp_added: integer({ mode: 'timestamp_ms' }).notNull(),
    //Last visit
    timestamp_visited: integer({ mode: 'timestamp_ms' }).notNull(),
});