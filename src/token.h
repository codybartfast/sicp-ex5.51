enum token_type{
	TKN_EOF = -1,
	TKN_NUMBER 
};

struct token {
	enum token_type type;
	char * value;
	long position;
};
