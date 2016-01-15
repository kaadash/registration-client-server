# HEADERS = program.h headers.h
# OBJECTS	=	patient.c registration.c doctor.c
OUTPUTS=patient registration doctor

default: build

build: $(OBJECTS)
	gcc patient.c -o patient
	gcc registration.c -o registration
	gcc doctor.c -o doctor

clean:
	@$(foreach OUTPUT, $(OUTPUTS), rm -f $(OUTPUT);)
