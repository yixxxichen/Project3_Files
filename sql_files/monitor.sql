drop table if exists monitor;
create table monitor(item char(20) primary key,c integer);
insert into monitor values('cnt',0);
update monitor set c=0 where item='cnt';


delimiter $$
drop trigger if exists enrollment_monitor $$
create trigger enrollment_monitor
after update on uosoffering
for each row
begin
    if (new.MaxEnrollment > 2*new.Enrollment)
    then update monitor set c=1 where item='cnt';
    elseif(new.MaxEnrollment <= 2*new.Enrollment)
    then update monitor set c=0 where item='cnt';
    end if;
end $$
delimiter ;
###call enroll_class('3213','COMP5348','2016','Q1');