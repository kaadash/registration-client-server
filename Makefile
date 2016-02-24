# HEADERS = program.h headers.h
# OBJECTS	=	patient.c registration.c doctor.c
OUTPUTS=patient registration doctor doctorTest registrationTest patientTest

default: build

build: $(OBJECTS)
	gcc patient.c -o patient -Wall
	gcc registration.c -o registration -Wall
	gcc doctor.c -o doctor -Wall
	gcc -include registrationFunctions.h registration.c -o registrationTest -Wall
	gcc -include structures.h patient.c -o patientTest -Wall
	gcc -include structures.h -include helpers.h doctor.c -o doctorTest -Wall

clean:
	@$(foreach OUTPUT, $(OUTPUTS), rm -f $(OUTPUT);)
