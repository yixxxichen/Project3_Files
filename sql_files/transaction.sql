delimiter $$
drop procedure if exists withdraw_class $$
create procedure withdraw_class(
	in sid char(20),
    in cid char(20),
	in y char(20),
	in sem char(20)
)
begin
delete from transcript where StudId=sid and UoSCode=cid and Semester=sem and Year=y;
update uosoffering set Enrollment = Enrollment -1 where UoSCode = cid and Semester=sem and Year=y;
end $$
delimiter ;
#call enroll_class('3213','COMP3520','2017','Q2');
###call withdraw_class('3213','COMP3520','2017','Q2');