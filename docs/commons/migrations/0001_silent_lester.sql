CREATE TABLE `bookmarks` (
	`id` integer PRIMARY KEY AUTOINCREMENT NOT NULL,
	`curated` integer NOT NULL,
	`notes` text,
	`tags` text,
	`timestamp_added` integer NOT NULL,
	`timestamp_visited` integer NOT NULL
);
--> statement-breakpoint
CREATE TABLE `logs` (
	`id` integer PRIMARY KEY AUTOINCREMENT NOT NULL,
	`session` integer NOT NULL,
	`level` integer NOT NULL,
	`type` integer NOT NULL,
	`what` text,
	`root` text,
	`path` text,
	`timestamp` integer NOT NULL
);
