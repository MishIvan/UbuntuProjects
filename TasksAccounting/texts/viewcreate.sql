CREATE VIEW "WorksView"
AS 
select t2.TaskID, t1.Name, t1.Deadline, t1.FulfillmentDate,
t2.ID WorkID, t2.Content, t2.Date, t2.TimeSpent 
from tasks t1 left join works t2 on t1.ID = t2.TaskID
order by t1.ID, t2.ID
