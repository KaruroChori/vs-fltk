import { sqliteTable, integer, blob, text } from "drizzle-orm/sqlite-core"

export const apps = sqliteTable('logs', {
    id: integer().primaryKey({ autoIncrement: true }),  //Unique id for the message
    session: integer().notNull(),                       //Session id, each instance of vs is a new session
    level: integer().notNull(),                         //Message level
    type: integer().notNull(),                          //Message type
    what: text(),                                       //Message explanation
    root: text(),                                       //Current app root
    path: text(),                                       //Current path in the app where the message was triggered
    timestamp: integer().notNull(),                     //When the message was recorded
});