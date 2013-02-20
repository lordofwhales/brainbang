/**
 * Robert Perce
 * bb.c - a brainbang interpreter
 * (C) 2012 Creative Commons BY-NC-SA
 *
 * Version 2.1
 */
#include <iostream>
#include <sstream>
#include <fstream>
#include <getopt.h>
#include "map.c"
using namespace std;

map<string,string> functions;

void couthelp() {
	cout <<
		"bb - a brainbang interpreter\n" <<
		"(C) 2013 MIT License\tRobert Perce\trobert.perce@gmail.com\n" <<
		"\nUsage information:\n" <<
		"\tbb [options] inputfile\n" <<
		"\nAvailable options:\n" <<
		"\t-s <num>\t\tNumber of cells (default 10000)\n" <<
		"\t--size <num>\n\n" <<
		"\t-v <bool>\t\tLoop values (default true).  If true, 0-1=255 and 255+1=0.\n" <<
		"\t--loopval <bool>\n\n" <<
		"\t-p <bool>\t\tLoop pointer (default false).  If true, at 0 '<' moves to the end.\n" <<
		"\t--looppoint <bool>\n\n" <<
		"\t-c\t\t\tOutput code (default false).  If true, outputs only brainfuck code.\n" <<
		"\t--codeoutput\n\n" <<
		"\t-n\t\t\tOutput as numbers, not characters (default false).  Newlines are still newlines.\n" <<
		"\t--numout\n\n" <<
		"\t-t\t\t\tDon't limit to 0-255 (default false): allow large and negative numbers.  Implies -n.\n" <<
		"\t--nolimit\n\n" <<
		"\t-h\t\t\tPrint this message\n" <<
		"\t--help" << endl;
}
string trim(string s) {
	string out = "";
	int start,end;
	for(start=0; start<s.length(); start++) 
		switch(s[start]) {
			case ' ':case '\t':case '\n':break;
			default:goto next;
		}
	next:
	for(end=s.length()-1; end>=0; end--)
		switch(s[end]) {
			case ' ':case '\t':case '\n':break;
			default:goto end;
		}
	end:
	return s.substr(start,end-start+1);
}

int getChars(char c,string file,int from) {
	int ct = 0;
	for(int i=from; i<file.length(); i++) {
		if(file[i]==c) ct++;
	}
	return ct;
} int getChars(char c,string file) {
	return getChars(c,file,0);
}

int indexOf(string longstr, string tgt, int from) {
	string str = longstr.substr(from);
	if(tgt=="") return 0;
	for(int i=0; i<str.length()-tgt.length()+1; i++) {
		if((str.substr(i,tgt.length()))==tgt) return i+from;
	}
	return -1;
} int indexOf(string str, string tgt) {
	return indexOf(str, tgt, 0);
}

int lastIndexOf(string longstr, string tgt, int from) {
	string str = longstr.substr(0,from);
	for(int i=str.length()-tgt.length(); i>=0; i--) {
		if(str.substr(i,tgt.length())==tgt) return i;
	}
	return -1;
} int lastIndexOf(string str, string tgt) {
	return lastIndexOf(str,tgt,str.length());
}

string load(string file) {
	string outfile = file, s, line, newfile;
	int fstBang, sndBang;
	char c;
	ifstream newstream;
	while(getChars('%',file)>1) {
		outfile = "";
		for(int i=0; i<file.length(); i++) {
			c = file[i];
			if(c=='%') {
				if(getChars('%',file,i)<2) {
					outfile += c;
					continue;
				}
				i = indexOf(file,"%",i+1);
			} else {
				outfile += c;
			}
		}
		file = outfile;
	}
	while(getChars('!',file)>2) {
		outfile = "";
		for(int i=0; i<file.length(); i++) {
			c = file[i];
			if(c=='!') {
				if(getChars('!',file,i)<3) {
					outfile += c;
					continue;
				}
				fstBang = indexOf(file,"!",i+1);
				sndBang = indexOf(file,"!",fstBang+1);
				if(file.substr(i+1,fstBang-i-1)=="load") {
					s=file.substr(fstBang+1,sndBang-fstBang-1);
					char * cs = new char[s.length()];
					for(int j=0; j<s.length(); j++) cs[j] = s[j+i];
					newstream.open(cs);
					if(newstream.is_open()) {
						while(!newstream.eof()) {
							getline(newstream,line);
							newfile += line + "\n";
						}
						newstream.close();
					}
					outfile += load(newfile);
				} else {
					functions.put(file.substr(i+1,fstBang-i-1),file.substr(fstBang+1,sndBang-fstBang-1));
				}
				i = sndBang;
			} else outfile += c;	
		}
		file = outfile;
	}
	while(getChars(':',file)>1) {
		outfile = "";
		for(int i=0; i<file.length(); i++) {
			c = file[i];
			if(c==':') {
				if(getChars(':',file,i)<2) {
					outfile += c;
					continue;
				}
				outfile += functions.get(file.substr(i+1,indexOf(file,":",i+1)-i-1));
				i = indexOf(file,":",i+1);
			} else {
				outfile += c;
			}
		}
		file = outfile;
	}
	return outfile;
}

unsigned int pow(unsigned int b,unsigned int e) {
	if(e==0) return 1;
	return b*pow(b,e-1);
}
int main(int len,char* args[]) {
	unsigned int size = 10000;
	bool loopval = true,looppoint = false;
	bool codeoutput = false, numout=false;
	bool nolimit = false;

	//<debug mode options>
	bool debug;
	int debug_first,debug_last;
	//</dmo>
	int c,t;
	opterr=0;
	static struct option longopts[] = {
		{"size",		required_argument,		0, 's'},
		{"loopval",		required_argument,	0, 'v'},
		{"looppoint",	required_argument,	0, 'p'},
		{"first",		required_argument,	0, 'b'},
		{"last",		required_argument,		0, 'l'},
		{"debug",		no_argument,			0, 'd'},
		{"codeoutput",	no_argument,			0, 'c'},
		{"numout",	no_argument,				0, 'n'},
		{"nolimit", no_argument,				0, 't'},
		{"help",		no_argument,				0, 'h'},
		{0, 0, 0, 0}
	};
	
	while(1) { //test two lines down
		c=getopt_long(len,args,"s:v:p:b:l:ntdch",longopts,0);
		if(c==-1) break;
		switch(c) {
			case 'n':
				numout=true;
				break;
			case 't':
				nolimit=true;
				numout = true;
				break;
			case 's':
				stringstream(optarg) >> size;
				if(size==0) { 
					cout << "Error: -s requires numerical argument 0 < size <= " << pow(2,8*sizeof(int))-1 << endl;
					return 1;
				} break;
			case 'v':
				stringstream(optarg) >> t;
				if(t!=1 && t!=0 && optarg!="true" && optarg!="false") {
					cout << "Error: -v requires boolean argument 0, 1, false, or true" << endl;
					return 1;
				} loopval = t;
				break;
			case 'p':
				stringstream(optarg) >> t;
				if(t!=1 && t!=0 && optarg!="true" && optarg!="false") {
					cout << "Error: -p requires boolean argument 0, 1, false, or true" << endl;
					return 1;
				} looppoint = t;
				break;
			case 'b':
				stringstream(optarg) >> debug_first;
				break;
			case 'l':
				stringstream(optarg) >> debug_last;
				break;
			case 'd':
				debug = true;
				break;
			case 'c':
				codeoutput = true;
				break;
			case 'h':
				couthelp();
				return 0;
				break;
			case '?':
				if(string("svpch").find(char(optopt))==string::npos)
					cout << "Error: unknown option -" << char(optopt) << endl;
				else
					cout << "Error: required argument for -"<< char(optopt) << endl;
				return 1;
				break;
			default:
				cout << "Error: can't." << endl;
		}
	}
	if(optind >= len) {
		couthelp();
		return 1;
	}
	string filetext,line,output;
	ifstream file (args[optind]);
	if(file.is_open()) {
		while(!file.eof()) {
			getline (file,line);
			filetext += line+"\n";
		}
		file.close();
	} else {
		cout << "+++ MELON MELON MELON +++" << endl;
		cout << "(That file doesn't exist)" << endl;
		return 1;
	}

	int p = 0,loopp=0,loop = 0;
	bool loopb = false;
	string bit;

	output = load(filetext);

	if(codeoutput) {
		cout << trim(output) << endl;
		return 0;
	}

	int a[size];
	for(int i=0; i<output.length(); i++) {
		switch(output[i]) {
			case '+': if(a[p]==255 && !nolimit) {
							if(loopval) a[p]=0;
						 } else a[p]++; break;
			case '-': if(a[p]==0 && !nolimit) {
							if(loopval) a[p]=255;
						 } else a[p]--; break;
			case '[': if(a[p]!=0) {
							loopb = true;
							loop = i;
						 } else i = indexOf(output,"]",i+1); break;
			case ']': loopb = false; i = (a[p]==0?i:loop); break;
			case '>': if(p==size) {
							if(looppoint) p=0;
						 } else p++; break;
			case '<': if(p==0) {
							if(looppoint) p=size;
						 } else p--; break;
			case '.': if(a[p]=='\n') cout << endl;
						 else if(numout) cout << int(a[p]);
						 else cout << a[p]; break;
			case ',': cin >> bit; a[p] = bit[0]; break;
		}
	}

	return 0;
}
