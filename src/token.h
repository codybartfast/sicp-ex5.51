enum token_type{
	TKN_NUMBER,
	TKN_EOF
};

struct token {
	enum token_type type;
	char * value;
	long position;
};
