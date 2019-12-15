CXX=g++
RM=rm -f

SRCS=interpreter.cpp expression.cpp client.cpp predicates.cpp table.cpp database.cpp sql.cpp rest.cpp language.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: tool

tool: $(OBJS)
	$(CXX) -o tool $(OBJS) 

depend: 
	makedepend -- $(SRCS)

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) *~ .depend

include .depend