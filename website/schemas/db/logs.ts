import { sqliteTable, integer, text } from "drizzle-orm/sqlite-core"

/**
 * Application logs. In place/addition of them being shown on the terminal, logs gets recorded.
 */
export const apps = sqliteTable('logs', {
    id: integer().primaryKey({ autoIncrement: true }),          //Unique id for the message
    session: integer().notNull(),                               //Session id, each instance of vs is a new session
    level: integer().notNull(),                                 //Message level
    type: integer().notNull(),                                  //Message type
    what: text(),                                               //Message explanation
    root: text(),                                               //Current app root
    path: text(),                                               //Current path in the app where the message was triggered
    timestamp: integer({ mode: 'timestamp_ms' }).notNull(),     //When the message was recorded
});