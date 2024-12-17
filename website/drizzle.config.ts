import { defineConfig } from "drizzle-kit";

export default defineConfig({
  dialect: 'sqlite',
  schema: './schemas/db',
  out: './commons/migrations',
  dbCredentials:{
    url:'./commons/db.sqlite'
  }
})