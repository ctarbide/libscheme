/* scheme_alloc.c */
Scheme_Object *scheme_alloc_object(void);
void *scheme_malloc(size_t size);
void *scheme_calloc(size_t num, size_t size);
char *scheme_strdup(char *str);
/* scheme_bool.c */
void scheme_init_bool(Scheme_Env *env);
int scheme_eq(Scheme_Object *obj1, Scheme_Object *obj2);
int scheme_eqv(Scheme_Object *obj1, Scheme_Object *obj2);
int scheme_equal(Scheme_Object *obj1, Scheme_Object *obj2);
/* scheme_char.c */
void scheme_init_char(Scheme_Env *env);
Scheme_Object *scheme_make_char(char ch);
/* scheme_env.c */
Scheme_Env *scheme_basic_env(void);
void scheme_add_global(char *name, Scheme_Object *obj, Scheme_Env *env);
Scheme_Env *scheme_new_frame(int num_bindings);
void scheme_add_binding(int index, Scheme_Object *sym, Scheme_Object *val, Scheme_Env *frame);
Scheme_Env *scheme_extend_env(Scheme_Env *frame, Scheme_Env *env);
Scheme_Env *scheme_add_frame(Scheme_Object *syms, Scheme_Object *vals, Scheme_Env *env);
Scheme_Env *scheme_pop_frame(Scheme_Env *env);
void scheme_set_value(Scheme_Object *symbol, Scheme_Object *val, Scheme_Env *env);
Scheme_Object *scheme_lookup_value(Scheme_Object *symbol, Scheme_Env *env);
Scheme_Object *scheme_lookup_global(Scheme_Object *symbol, Scheme_Env *env);
/* scheme_error.c */
void scheme_init_error(Scheme_Env *env);
void scheme_signal_error(char *msg, ...);
void scheme_warning(char *msg, ...);
void scheme_default_handler(void);
/* scheme_eval.c */
void scheme_init_eval(Scheme_Env *env);
Scheme_Object *scheme_eval(Scheme_Object *obj, Scheme_Env *env);
/* scheme_fun.c */
void scheme_init_fun(Scheme_Env *env);
Scheme_Object *scheme_make_prim(Scheme_Prim *fun);
Scheme_Object *scheme_make_closure(Scheme_Env *env, Scheme_Object *code);
Scheme_Object *scheme_make_cont(void);
Scheme_Object *scheme_apply(Scheme_Object *rator, int num_rands, Scheme_Object **rands);
Scheme_Object *scheme_apply_to_list(Scheme_Object *rator, Scheme_Object *rands);
/* scheme_hash.c */
Scheme_Hash_Table *scheme_hash_table(int size);
void scheme_add_to_table(Scheme_Hash_Table *table, char *key, void *val);
void *scheme_lookup_in_table(Scheme_Hash_Table *table, char *key);
void scheme_change_in_table(Scheme_Hash_Table *table, char *key, void *new);
/* scheme_list.c */
void scheme_init_list(Scheme_Env *env);
Scheme_Object *scheme_make_pair(Scheme_Object *car, Scheme_Object *cdr);
Scheme_Object *scheme_alloc_list(int size);
int scheme_list_length(Scheme_Object *list);
Scheme_Object *scheme_map_1(Scheme_Object *(*fun)(Scheme_Object *), Scheme_Object *lst);
Scheme_Object *scheme_car(Scheme_Object *pair);
Scheme_Object *scheme_cdr(Scheme_Object *pair);
Scheme_Object *scheme_cadr(Scheme_Object *pair);
Scheme_Object *scheme_caddr(Scheme_Object *pair);
/* scheme_number.c */
void scheme_init_number(Scheme_Env *env);
Scheme_Object *scheme_make_integer(int i);
Scheme_Object *scheme_make_double(double d);
/* scheme_port.c */
void scheme_init_port(Scheme_Env *env);
Scheme_Input_Port *scheme_make_input_port(Scheme_Object *subtype, void *data, int (*getc_fun)(Scheme_Input_Port *), void (*ungetc_fun)(int, Scheme_Input_Port *), int (*char_ready_fun)(Scheme_Input_Port *), void (*close_fun)(Scheme_Input_Port *));
Scheme_Output_Port *scheme_make_output_port(Scheme_Object *subtype, void *data, void (*write_string_fun)(char *str, Scheme_Output_Port *), void (*close_fun)(Scheme_Output_Port *));
int scheme_getc(Scheme_Object *port);
void scheme_ungetc(int ch, Scheme_Object *port);
int scheme_char_ready(Scheme_Object *port);
void scheme_close_input_port(Scheme_Object *port);
void scheme_close_output_port(Scheme_Object *port);
Scheme_Object *scheme_make_file_input_port(FILE *fp);
Scheme_Object *scheme_make_string_input_port(char *str);
Scheme_Object *scheme_make_file_output_port(FILE *fp);
/* scheme_print.c */
void scheme_debug_print(Scheme_Object *obj);
void scheme_write(Scheme_Object *obj, Scheme_Object *port);
void scheme_display(Scheme_Object *obj, Scheme_Object *port);
char *scheme_write_to_string(Scheme_Object *obj);
char *scheme_display_to_string(Scheme_Object *obj);
void scheme_write_string(char *str, Scheme_Object *port);
/* scheme_promise.c */
void scheme_init_promise(Scheme_Env *env);
Scheme_Object *scheme_make_promise(Scheme_Object *expr, Scheme_Env *env);
/* scheme_read.c */
Scheme_Object *scheme_read(Scheme_Object *port);
/* scheme_string.c */
void scheme_init_string(Scheme_Env *env);
Scheme_Object *scheme_make_string(char *chars);
Scheme_Object *scheme_alloc_string(int size, char fill);
/* scheme_struct.c */
void scheme_init_struct(Scheme_Env *env);
Scheme_Object *scheme_apply_struct_proc(Scheme_Object *sp, Scheme_Object *args);
/* scheme_symbol.c */
void scheme_init_symbol(Scheme_Env *env);
Scheme_Object *scheme_make_symbol(char *name);
Scheme_Object *scheme_intern_symbol(char *name);
/* scheme_syntax.c */
void scheme_init_syntax(Scheme_Env *env);
Scheme_Object *scheme_make_syntax(Scheme_Syntax *proc);
/* scheme_type.c */
void scheme_init_type(Scheme_Env *env);
Scheme_Object *scheme_make_type(char *name);
/* scheme_vector.c */
void scheme_init_vector(Scheme_Env *env);
Scheme_Object *scheme_make_vector(int size, Scheme_Object *fill);
Scheme_Object *scheme_vector_to_list(Scheme_Object *vec);
Scheme_Object *scheme_list_to_vector(Scheme_Object *list);
