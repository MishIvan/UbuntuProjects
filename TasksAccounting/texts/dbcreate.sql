CREATE TABLE IF NOT EXISTS "Tasks"
(
	"ID"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"Name"	TEXT NOT NULL UNIQUE,
	"Content"	TEXT,
	"Deadline"	TEXT NOT NULL DEFAULT '9999-12-31',
	"FulfillmentDate"	TEXT,
	"Plan"	TEXT,
	"Fact"	TEXT
);

CREATE TABLE IF NOT EXISTS "Works"
(
	"ID"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
	"TaskID"	INTEGER NOT NULL,
	"Content"	TEXT NOT NULL,
	"Date"	TEXT NOT NULL,
	"TimeSpent"	TEXT NOT NULL DEFAULT '00:00:00',
	FOREIGN KEY("TaskID") REFERENCES "Tasks"("ID")
);

CREATE VIEW "WorksView"
AS
select t2.TaskID, t1.Name, t1.Deadline, t1.FulfillmentDate,
t2.ID WorkID, t2.Content, t2.Date, t2.TimeSpent
from tasks t1 left join works t2 on t1.ID = t2.TaskID
order by t1.ID, t2.ID;