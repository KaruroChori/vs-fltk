CREATE TABLE `apps` (
	`id` integer PRIMARY KEY AUTOINCREMENT NOT NULL,
	`app_id` blob NOT NULL,
	`base` text NOT NULL,
	`notes` text,
	`source` text
);
--> statement-breakpoint
CREATE UNIQUE INDEX `apps_app_id_unique` ON `apps` (`app_id`);--> statement-breakpoint
CREATE TABLE `policies` (
	`id` integer PRIMARY KEY AUTOINCREMENT NOT NULL,
	`app_id` blob,
	`data` text NOT NULL
);
--> statement-breakpoint
CREATE UNIQUE INDEX `policies_app_id_unique` ON `policies` (`app_id`);--> statement-breakpoint
CREATE TABLE `secrets_shared` (
	`id` integer PRIMARY KEY NOT NULL,
	`from_app` integer NOT NULL,
	`with_app` integer NOT NULL,
	`tag` text,
	`notes` text,
	`timestamp_creation` integer NOT NULL,
	`timestamp_expiring` integer NOT NULL,
	FOREIGN KEY (`from_app`) REFERENCES `apps`(`id`) ON UPDATE cascade ON DELETE cascade,
	FOREIGN KEY (`with_app`) REFERENCES `apps`(`id`) ON UPDATE cascade ON DELETE cascade
);
--> statement-breakpoint
CREATE UNIQUE INDEX `secretsUnique` ON `secrets_shared` (`from_app`,`with_app`,`tag`);--> statement-breakpoint
CREATE TABLE `secrets` (
	`id` integer PRIMARY KEY NOT NULL,
	`app` integer NOT NULL,
	`tag` text,
	`path` text NOT NULL,
	`value` blob NOT NULL,
	FOREIGN KEY (`app`) REFERENCES `apps`(`id`) ON UPDATE cascade ON DELETE cascade
);
--> statement-breakpoint
CREATE UNIQUE INDEX `secretsUniqueIdx` ON `secrets` (`app`,`tag`,`path`);--> statement-breakpoint
CREATE TABLE `storage_kv` (
	`id` integer PRIMARY KEY NOT NULL,
	`app` integer NOT NULL,
	`key` text NOT NULL,
	`value` blob NOT NULL,
	`timestamp_creation` integer NOT NULL,
	`timestamp_expiring` integer NOT NULL,
	FOREIGN KEY (`app`) REFERENCES `apps`(`id`) ON UPDATE cascade ON DELETE cascade
);
--> statement-breakpoint
CREATE UNIQUE INDEX `kvUniqueIdx` ON `storage_kv` (`app`,`key`);--> statement-breakpoint
CREATE TABLE `storage_res` (
	`id` integer PRIMARY KEY AUTOINCREMENT NOT NULL,
	`app` integer NOT NULL,
	`src` text NOT NULL,
	`content` blob NOT NULL,
	`hash` blob NOT NULL,
	`timestamp_creation` integer NOT NULL,
	`timestamp_expiring` integer,
	FOREIGN KEY (`app`) REFERENCES `apps`(`id`) ON UPDATE cascade ON DELETE cascade
);
--> statement-breakpoint
CREATE UNIQUE INDEX `resUniqueIdx` ON `storage_res` (`app`,`src`);