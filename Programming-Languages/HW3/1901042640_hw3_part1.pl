%            PART 1.

% This part is for being able to assert
:- dynamic (student / 3).
:- dynamic (course / 7).
:- dynamic (room / 5).

% knowledge base

%Order : id of the room, capacity of the room, hours, equipment, access for the handicapped students.
room(z06,10,[8,9,10,11,12,13,14,15,16], smartboard, handicapped).
room(z10, 15,[8,9,10,11,12,13,14,15,16], projector, handicapped).
room(z23,20,[8,9,10,11,12,13,14,15,16], none, none).

%Order : id of the instructor, course ids, equipment.
instructor(0, [cse321], smartboard).
instructor(1, [cse331], none).
instructor(2, [cse341], projector).
instructor(3, [cse343], smartboard).

%Order : id of the course, id of the instructor ,capacity of the course, id of the room, student ids that enrolled the course, equipment
course(cse321, 0, 7, [14,15,16], z06, [1,4,8], smartboard).
course(cse331, 1, 5, [13,14,15], z23, [9,10,11,12,13], none).
course(cse341, 2, 9, [8,9,10,11], z10, [1,3,7,8,9,12,13], projector).
course(cse343, 3, 6, [14,15,16], z06, [2,3,6,7], smartboard).

% It is for keeping room’s occupancy information.
% Order : id of the room , if of the course, hours for the course.
occupancy(z06, cse321, [14,15,16]).
occupancy(z23, cse331, [13,14,15]).
occupancy(z10, cse341, [8,9,10,11]).
occupancy(z06, cse343, [14,15,16]).

% Order : id of the student, courses that student takes, handicapped information.
student(1,[cse341,cse321], none).
student(2,[cse343],none).
student(3,[cse343,cse341],none).
student(4,[cse321],none).
student(5,[],none).
student(6,[cse343],handicapped).
student(7,[cse341,cse343],none).
student(8,[cse321,cse341], handicapped).
student(9,[cse331, cse341], none).
student(10, [cse331], none).
student(11, [cse331], none).
student(12,[cse331, cse341], none).
student(13,[cse331,cse341], none).

%  rules

% Adds student to the system
add_student(ID,Courses, Handicapped):- assertz(student(ID, Courses, Handicapped)).

% Adds course to the system
add_course(ID, Instructor, Capacity, Hours, RoomID, StudentIDs, Equipment, Handicapped):- assertz(course(ID, Instructor, Capacity, Hours, RoomID, StudentIDs, Equipment, Handicapped)).

% Adds room to the system
add_room(ID, Capacity, Hours, Equipment, Handicapped):- assertz(ID, Capacity, Hours, Equipment, Handicapped).

% Checks if 2 given courses timelines crosses. Completes this task by using recursive function common_elements.
check_conflict(CourseID1, CourseID2):- occupancy(_, CourseID1, Hours1), occupancy(_, CourseID2, Hours2), (common_elements(Hours1, Hours2)-> true;  common_elements(Hours1, Hours2)). 

% Checks if given room has provides the given course's needs. Needs are capacity, equipment and access for handicappeds.
check_assign(RoomID, CourseID) :- course(CourseID, InstructorID, Course_Capacity, _, _, StudentList, _),	% Checks if there is a course with the given CourseID
                            instructor(InstructorID , _, Equipment),										% Checks if there is a instructor with the given CourseID's InstructorID
                            (
                                Equipment == none ->
                                    (     
                                          is_there_handicapped(StudentList) ->                    % Checks if there is handicapped student in the course                                          
                                        	room(RoomID, Room_Capacity, _, none, handicapped);         % If there is handicapped student and no needed equipment
                                        	room(RoomID, Room_Capacity, _, none, none)                    % If there is no handicapped student and there is no needed equipment                                       
                                    );
                                    (
                                    	is_there_handicapped(StudentList) ->                       % Checks if there is handicapped student in the course
                                        room(RoomID, Room_Capacity, _, Equipment, handicapped); % If there is handicapped student and needed equipment
                                        room(RoomID, Room_Capacity, _, Equipment, none)             % If there is no handicapped student and there is needed equipment
                                    )                              
                           ),
                           Room_Capacity >= Course_Capacity.                                    % Compares capacities of room and course. True if room has greater or equal capacity than course. Else, false

% Checks if there is enough capacity for a new student. If that student is handicapped checks if course has access for handicappeds.
check_enroll(StudentID, CourseID):- student(StudentID, Course_List, Handicapped),						% Checks if there is a student with the gicen StudentID 
                                   course(CourseID, _, Course_Capacity, _, RoomID, Student_List, _),	% Checks if there is a course with the gicen CourseID                               
                                   (                               
                                        Handicapped = handicapped ->                            
                                        room(RoomID, _, _, _, Handicapped);                     		% If the student is handicapped
                                        room(RoomID, _, _, _, _)                                		% If the student is not handicapped
                                   ),
                                   (length(Student_List, Y), Y < Course_Capacity),             		 	% Checks if there is enough space for a new student. Which means capacity is not full
                                   not(check_conflicted_courses(CourseID, Course_List)).         		% Checks if the given course's timeline crosses with another course that given student takes

% Checks if 2 lists have any common elements
 
common_elements([H | _], L) :- member_equal(H, L).  
common_elements([_ | T], L) :- common_elements(T, L).  
member_equal(X, [H | _]) :- X == H.  
member_equal(X, [_ | T]) :- member_equal(X, T).
    
% Checks if given course conflicts with any of the courses that student takes
check_conflicted_courses(CourseID, [H | T]):- (check_conflict(CourseID, H) -> true; check_conflicted_courses(CourseID, T)).
                                                  
% Checks is there is a handicapped student in the input list
is_there_handicapped([H | T]) :- student(H, _, Handicapped), (Handicapped = handicapped -> true; is_there_handicapped(T)).