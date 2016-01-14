# OnePunch-Chat
2015年中興大學資工系 視窗程式設計課程期末project -- 一拳通（OnePunch Chat）
Course: Windows Programming
School: National Chung Hsing University
Department: Computer Science & Engineering
Team: 5
Author: 陳勇瑜、葉日豐
Complete date: 12/1/16
一拳通 OnePunch Chat
Introduction:
	OnePunch Chat is a MFC-based windows program, which is constructed in a master-slave structure. Furthermore, we implement “CAsyncSocket” class, which is a library of Windows Socket provided by “Microsoft Foundation Classes”. It is inherited from “CObject" Class in a hierarchical relationship.
	OnePunch Chat mainly includes two programs: the “Server” and the “Client”. With respect to the Server, it is basically used to setup as a web server to listen the incoming knocking to the decided port. Also, it is in charge of the data distribution to the clients as well as the handling of some errors, debugs, and information sent from clients. 
	The other way around, the “Client”. This client side program is the primary user interface that we provide to the users. In the program, three dialogs will be shown to the users, user list, chat room, and login dialogs. Once the inputting IP address and port number in the client program are identical with the server one, the client will connect to the sever and the chat starts!

Feature:	
Two of the Server and Client programs are implemented in MFC which is not depend on .NET framework.
In Server, different types of log information are colored different ways to be more specific to understand.
In Client, we provide several status choices for the users:
Several profile pictures for user to be on the status list.
You can input your current mood which will be shown on everyone’s user list dialog.
We also show if your friends are currently on line.

Goal:
	To be straight, we hope that we can build our project as close as to the Yahoo Messenger. Yahoo Messenger is the young memory of two of us, and we would like to make good use of this rare opportunity to dig into the whole background knowledge of the Yahoo Messenger and further understand the building procedure and finally implement a totally brand new version on our own. 
	In the Function, we hope that we can complete the following functions:
Group chat: 
There is a main chatting dialog with the online user list beside, and by logging into the server 		and sending the typing stuffs, everyone can simultaneously chat from and to whom is currently connecting to the server by the program.
Private chat:
By clicking the list control of the friend list, you can chat with the specific friend of you privately. 
Send media:
We hope that we can achieve the media transmission by socket programming. (NOT COMPLETE)
Multimedia streaming:
The ultimate goal of our project is to have the live audio chatting and video chatting function, and even live broadcasting, in order to compete with the mighty company of messenger at the moment, such as: Skype, Facebook … and so on. (NOT COMPLETE)

Bountifully innovative UI/UX design:
This is quite a challenging area that we’re neither a designer nor taking any corresponding classes. However, we tried our best to critique ourselves whether the software is user-friendly or not, rendering it a better message software than others.

Author & Member contribution:

104 Windows Programming Class 5th team:
 
4101056034 資工三 陳勇瑜100%:
Mainly Logical part of the project:
Define the data structure of the packets:
login
user information
Socket programming:
connect
bind
receive
send
listen
accept
Group chatting function
Message delivering between dialogs
postmessage()
Slides Making

4102056015 資工三 葉日豐100%:
Both Logical part and User Interface part of the project:
UI design including:
login dialog
chatting dialog
user list and status dialog
Logic:
data structure of 
is online
profile picture
current mood

Completed on 2016/1/12

Code Description:






Fig: Server’s classes diagram					Fig: Client’s classes diagram

	OnReceiver(…) will be called when the client is sending data, and after getting the data, the program will call OnEventTCPSocketRead() to read the data structure of incoming data. Including: login message, chat message, and status-changing message. Each of the message will be broadcasted to each client by the server in order to update the messenger dialog and chat room dialog.
	


Some important UI functions.
Messenger is inherited from CListBox, and we change the format to a two-line style, with one for name and the other for status. We also contain profile picture and isOnline status which are all done in the DrawItem(). Finally, we use SendData() to inform clients to update.

In the chat room, we use some .DLL file written by others to implement Emoticons. By clicking the Emoticon, it’ll be transform into a unique text, and then if sent out, the Emoticon will show up on the RichEdit.


Content & Usage(Results):


Fig.1 In the Server side, type a unoccupied port and maximum data size, and then press start to open the server.
You’ll probably see the right diagram. That’s the accessing announcement from Windows.


Fig.2 In the Client part, you’ll first see a log in view. You need to input the correspondent IP address and port of the server, and input your username and password.
Fig.3 And then you log in!
Fig.4 After pressing the 「確定」button, you’ll see the second dialog “Messenger”. In the Messenger, you can see the user list which shows the users that is currently in the server. To add up, you can use the profile button to change your profile picture, and update you mood by typing in the edit box!

Fig.5 You can also change the current status and profile pictures of yourself.




Fig.6 And definitely other’s can see your change.
Fig.7 Also, in the Chatroom dialog, you can change the fonts, color, size, and even styles if you want to.

Fig.8 Last but not least, we maintain the most memorable option of  Yahoo! Messenger, the knock knock! by clicking the knock knock. You are able to shake other’s chatting dialog and then to wake someone up!


Feedback:


4101056034 資工三 陳勇瑜：

	Second time trying to write a MFC project, we seek to learn this powerful and widely used windows programming skill at the moment. This is my second opportunity to learn a GUI language, besides Android. So many things are familiar to me, just like they are the same language. However, I still spend a bunch of time to deal with it. After those struggles, so intensive can I feel how a GUI program functions. Make me a better windows programmer and insightful when being faced with a windows program in front of me.

	 I now not only get to know the how to drag those fancy UI, but also improve my the ability of 
dealing with network connection logic. De facto, I was me who raise the idea of this topic(I’m so sorry that I pushed my partner into such a big hole like this. XD). When I first step into coding world, different from others who have the interests to build the game, I always want to know how hackers break others system by hacking into others system, and all of the techniques behind it. I decisively want to seize this chance to  
to learn socket programming. The process is struggled, however, one time, when I find the website of a course of NTUEE, building socket chat room is just one of their homework! I was totally astonished, and I know I have to get this done!

	Finishing up my first cooperative project, I felt it so much surreal. So much debugging, merge conflicts,  nightmares occurred to me,  nonetheless, when I saw the awesome outcomes carried out, there were full of  gratitude and appreciation in my mind. I would like to show my highest appreciation to my partner 日豐, he totally carried me through this project. I was so fortunate to have him to be my partner. But I still owe him a 雞排 lol.

	After the whole semester, I have to say, I’m not a complacent person, but I feel so replete. Not just the C++ coding part, professor told us a lot about how to live a life, how important it is to have a principle of our life attitude. I believe that deeply shows that how much effort he put in this course! Consequently, in the near future, I hope that I can use coding to live a better life!
 	
	

4102056015 資工三 葉日豐：

	To be honest, I have forgotten why we decide to use MFC to do our final project. In the beginning, I had no idea how to do this. Even making a friend list is very difficult to me. It took me much time to finish it. There are something what I want or some problems what I face are hard to search through the Internet. It was a little frustrated me. But it was fulfilled and satisfied after conquering them. As time went by, I found that MFC is really powerful as professor said(but very troublesome...). Although I don’t fully complete this program, failed my final exam, stayed up late, I’m glad that I can make a windows program by myself and my teammate.

