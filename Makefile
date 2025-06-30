NAME=ircserv

MY_SOURCES= main.cpp \
			Server.cpp \
			Channel.cpp \
			Client.cpp \
			cmd/Command.cpp \
			cmd/Invite.cpp \
			cmd/Kick.cpp \
			cmd/Mode.cpp \
			cmd/Topic.cpp \
			cmd/Join.cpp \
			cmd/Privmsg.cpp \
			cmd/Part.cpp \
			cmd/Quit.cpp \
			cmd/Ping.cpp \
			cmd/Who.cpp \
			cmd/Nick.cpp

green='\033[0;32m'
light_cyan='\033[1;36m'
white='\033[1;37m'
green_c='\033[6;32m'
nc='\033[0m'

MY_OBJECTS=$(MY_SOURCES:%.cpp=%.o)

RM=rm -f

CXX=c++
CXXFLAGS=-Wall -Werror -Wextra -std=c++98

all: $(NAME)

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@
$(NAME) : $(MY_OBJECTS)
	@clear
	@$(CXX) $(CXXFLAGS) $^ -o $(NAME)
	@echo $(light_cyan)"\t   »»-————　Make　————-««\n"$(nc)



clean:
	@clear
	@$(RM) $(MY_OBJECTS)
	@echo $(light_green)"\t  »»-————　Clean　————-««\n"$(nc)

fclean: clean
	@$(RM) $(NAME)
	@echo $(white)"\t»»-————　Full clean　————-««\n"$(nc)

re: fclean all
	@echo $(light_green)"\t  »»-————　Clean　————-««\n"$(nc)
	@echo $(white)"\t»»-————　Full clean　————-««\n"$(nc)
	@echo $(green_c)"\t   »»-————　Done　————-««"$(nc)

.PHONY: all fclean clean re
