#!/bin/env bun
import { $ } from "bun"

//TODO: Very bad. It should be made more portable, not like this mess.
await $`cp -r ../commons ./`;