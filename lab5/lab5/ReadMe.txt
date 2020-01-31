Lab 5 Readme.

Group members: 
1. Adeel Shaikh
2. Terell Cornell
3. Andrew Han

// answer the following questions
1. Part 2.1: Adding events to the calendar
Think about the separation of concerns in the project. 
What class/classes is responsible for the user interface of the Calendar?
-Calendar Interface

What class/classes is responsible for managing the representation and construction of the Calendar?
-Full Calendar Builder


Which class should handle collecting information from the user about an event?
-Calendar Interface

Which class should handle constructing and adding the event to the Calendar?
-Calendar Interface



2. Part 2.2 Make the calendar interactive
How did you decide to efficiently search for an event by name?
-Using a multi-map that stores all the events with the event name as the key.


Which object in the project understands the structure of the Calendar? Which object should be responsible for jumping to a specific object in the Calendar?
-The CalendarBuilder is the object that understands the structure of the Calendar, so it should be responsible for jumping to a specific object. 


How did you design saving/restoring the calendar to/from a file? What information needs to be saved about a Calendar to successfully restore it?
-For saving the calendar to a file, the calendar interface writes the calendar's name number of years it stores on the same line separated by a comma, and after that,
 each event that is stored within the calendar is saved to the file by printing the calendar's name, and all the date info on one line before ending that line to print 
 the next event. When restoring the file from the calendar, the program first reads the name and the number of years and builds the calendar with that information as a full
 calendar builder would. Then, it reads all of the event lines, creating events with the with the information on each line, and adds them to the calendar. To successfully restore
 a calendar, the only information the file builder needs is for the first line to be formatted correctly, which is the name and number of years with a space in between. 





3. Part 2.3 Updating the display
How a particular component in the calendar should be displayed depends on what the current display of the calendar is.
How did you implement this functionality? 
-We used an abstract visitor class and multiple concrete visitors. We created one for each of the displayable components(Calendar, year, month, day, event).
 Each of the conctrete visitor classes behaves differently depending on what was passed into it's visit() function. For example, the monthDisplayVisitor only had to define
 the visit function for the days and the month itself, but defines visiting a calendar and a year and an event as returning without doing anything. 






Did you make any changes to the interfaces provided in the starter code? If so, how easy was it to 
refactor the code to match this change? 
-Yes we did. We had to add an accept method into the DisplayableComponent base class. This way, all we had to do was override the accept method for each of the calendarComponents
	to call visit on itself with the appropriate visitor, and then the accept function of all of its children.
 





How does your implementation maintain flexibility and extensibility of the project? Compare this with other designs you considered.
-Everything new that we added except for the visitors inherits from a base class that was already given to us. If we wanted to add another type of builder, all we have to do 
 is create another concrete builder class that inherits from the base class, and add another if statement in the CalendarInterface.cpp file that sets the builder member to that new builder. 
 We programmed to an interface instead of implementation






Part 3 (only answer the questions pertaining to the functionality you chose to implement)
3.1: Merging calendars
Describe the benefits of the Decorator pattern? If you used the decorator pattern, how was it applied to 
support merging calendars?






If you chose not to use the decorator pattern, how does your design compare with it when 
considering flexibility and extensibility of the design? 







3.2 ToDo List
Describe your implementation? How does it support easy extensibility of the project in the future?
How is the singleton pattern used to enforce only a sigle TODO list instance?
-When the user inputs a value that indicates they want to see the todoList, the program first checks is the todolist pointer object inside of the calendar class is null. If it is,
 it creates a new todo list and returns it. If not, it returns that member variable, successfully utilizing the singleton pattern. 

-We created a ToDoList class that inherits from DisplayableComponent, and we also created a Task Class that inherits from DisplayableComponent. Because both of these classes inherit from
 DisplayableComponent, implementing the functionality was easy. We didn't even have to define the addComponent function to the ToDoList class, as it inherits that from the 
 DisplayableComponent class. All we had to do was define the display and accept functions for each class. For marking the tasks complete, we just created a boolean member 
 variable inside of the Task Class, and whenever display is called, it checked that boolean and if it was true, printed " COMPLETE" After printing the name of the task and 
 it's date info. The way the display function uses the boolean member variable makes the ToDoList easily extensible. If we wanted to add functionality to make a task a priority, 
 we can just add another member variable to the task class that tells us whether it is a priority. Inside the display function, if the priority member is true, we can print "*" next
 to the task's information, and we can add another function in the ToDoList class called prioritize with the index of the chosen task. That function will simply set the isPriority 
 member to true and the isComplete member to false;







3.3 Incremental Builder
What challenges did you run into while implementing this? Were any changes to the builder interface required? 
How does using the builder pattern contribute to easily being able to change how an object(calendar in this case)
is represented?
-The incremental builder was the most difficult part of this lab. We began by using most of the same code as full calendar builder and then modifying it. 
We started by the build calendar, which, in this case, would just create a single day. Then we had to write some new functions such as build remaining days,
which makes the objects that have not yet been created, and it makes them whenever they need be accessed. This way, we build the calendar incrementally. 
-The builder has a very important effect on how the object is represented. When we had a full builder only, we were creating all the members that are part of
the object (calendar in this case). With the incremental builder, only the objects that are being accessed are created. This way we save memory space. 