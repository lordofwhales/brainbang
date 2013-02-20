%=============================================================
	Hello and welcome to Robert's Brainbang standard library!

	Brainfuck is fun, but it's really hard to do /anything/
	meaningful in.  Esoteric languages are cooler when you
	can actually get them to do something.  And thus I
	invented Brainbang, basically Brainfuck with functions
	and comments.  Why's it called Brainbang?
		1) Functions are deliniated with bangs, i.e. !
		2) A less emphatic fuck is a bang.  Brainbang hurts
			your head, but is actually understable.  Sorta.
	
	Function definition is of the form !<word>!<code>!. If
	<word> is "load", without quotes, brainbang will look 
	for a file of that name and execute all its code verb-
	atim wherever load was called.  Useful for creating 
	library files, like this one.

	Function calling is of the form :<word>: where <word>
	is an already-defined function.  If the function is not
	defined, nothing happens and the code is ignored.

	Comments are of the form %%text%%, where text can
	contain any character except percent signs and those %%
	are replaced with single percent signs. As seen by the
	special characters (, and ., for example) that you're
	reading that don't produce output, anything inside % %
	isn't even looked	at by	the compiler.   
	
	If comments are just alphanumerical, you don't need percents,
	only if you want to use commas/periods/any one of []<>.,+-!:
	
	For functions, try to describe what's going on in a comment.
	The comment can be as simple as that for bork, below, or more
	usefully copyleft: %% __. -> _.. %% (pretend those are single
	percent signs). This simply means "two zero cells and a cell
	with something in it results in one zero cell and two cells
	with the first something in them both".  You can also do it
	multiline to show pointer movement:
	  %% __. -> _.. %%
	  %%   ^      ^ %% (please mentally substitute single percents)
	In general, functions should but by no means are required to put
	the pointer back where it started.  You can set these up however
	you like, or not at all, but in general _ is an empty cell and .
	is nonzero.  If you use other characters, explain to that effect.
		
	Have fun!

=====================================================================%

!bork!+[>+]! 	% Put ones in ALL THE SLOTS! %
!zero![-]!		
!5!+++++!
!10!:5::5:!
!20!:10::10:!
!50!:20::20::10:!
!-5!-----!
!-10!:-5::-5:!
!-20!:-10::-10:!
!-50!:-20::-20::-10:!
!input!:zero:++:50::10:.:-20::-10:.:zero:,!  % Displays "> " and takes input %
!n!:zero::10:.!

!copyleft![<+<+>>-]<<[>>+<<-]>>!		% __. -> _.. %
!copyright![>+>+<<-]>>[<<+>>-]<<!	% .__ -> .._ %
!moveleft![<+>-]!		% _. -> ._ %
!moveright![>+<-]!	% ._ -> _. %

!if![!
!fi!:zero:]!
