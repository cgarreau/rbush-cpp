.PHONY: clean All

All:
	@echo "----------Building project:[ rbush - Release ]----------"
	@"$(MAKE)" -f  "rbush.mk"
clean:
	@echo "----------Cleaning project:[ rbush - Release ]----------"
	@"$(MAKE)" -f  "rbush.mk" clean
