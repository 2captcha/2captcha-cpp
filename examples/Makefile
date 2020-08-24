INC_DIR	+= \
	-I/usr/local/include \
	-I../ \

LIB_DIR	+= -L. \
		-L/usr/local/lib \
		-L/usr/lib \

DEFINES += -DAPI_KEY=\"$(API_KEY)\"

LIB_FILES = -lcurl -lcares -lssl -lcrypto -lz -lpthread -lidn2 

OPTIM		= -O3 -g 

GPP		= g++

CC_OPT		= $(OPTIM) -std=c++11 -Wall -pipe $(INC_DIR)

all:	normal canvas capy coordinates funcaptcha geetest grid hcaptcha keycaptcha recaptcha rotate text

normal:		normal.o
		$(GPP) ${OPTIM} -Wall -o normal normal.o $(LIB_DIR) $(LIB_FILES)

canvas:		canvas.o
		$(GPP) ${OPTIM} -Wall -o canvas canvas.o $(LIB_DIR) $(LIB_FILES)

capy:		capy.o
		$(GPP) ${OPTIM} -Wall -o capy capy.o $(LIB_DIR) $(LIB_FILES)

coordinates:	coordinates.o
		$(GPP) ${OPTIM} -Wall -o coordinates coordinates.o $(LIB_DIR) $(LIB_FILES)

funcaptcha:	funcaptcha.o
		$(GPP) ${OPTIM} -Wall -o funcaptcha funcaptcha.o $(LIB_DIR) $(LIB_FILES)

geetest:	geetest.o
		$(GPP) ${OPTIM} -Wall -o geetest geetest.o $(LIB_DIR) $(LIB_FILES)

grid:		grid.o
		$(GPP) ${OPTIM} -Wall -o grid grid.o $(LIB_DIR) $(LIB_FILES)

hcaptcha:	hcaptcha.o
		$(GPP) ${OPTIM} -Wall -o hcaptcha hcaptcha.o $(LIB_DIR) $(LIB_FILES)

keycaptcha:	keycaptcha.o
		$(GPP) ${OPTIM} -Wall -o keycaptcha keycaptcha.o $(LIB_DIR) $(LIB_FILES)

recaptcha:	recaptcha.o
		$(GPP) ${OPTIM} -Wall -o recaptcha recaptcha.o $(LIB_DIR) $(LIB_FILES)

rotate:		rotate.o
		$(GPP) ${OPTIM} -Wall -o rotate rotate.o $(LIB_DIR) $(LIB_FILES)

text:		text.o
		$(GPP) ${OPTIM} -Wall -o text text.o $(LIB_DIR) $(LIB_FILES)

.cpp.o:
		$(GPP) $(CC_OPT) $(DEFINES) -o $@ -c $< 

clean:
		rm -rf core *~ *.o normal canvas capy coordinates funcaptcha geetest grid hcaptcha keycaptcha recaptcha rotate text

strip:		$(PROG)
		strip $(PROG)
