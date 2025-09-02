// tests/test_lex.c
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// declare the lexer under test
char **lex(const char *s);

// --- tiny test helpers -------------------------------------------------------

static void free_tokens(char **toks) {
    if (!toks) return;
    for (size_t i = 0; toks[i]; ++i) free(toks[i]);
    free(toks);
}

// static void dump_tokens(const char *label, char **toks) {
//     fprintf(stderr, "%s: [", label);
//     if (!toks) { fprintf(stderr, "NULL]\n"); return; }
//     for (size_t i = 0; toks[i]; ++i) {
//         if (i) fprintf(stderr, ", ");
//         fprintf(stderr, "\"%s\"", toks[i]);
//     }
//     fprintf(stderr, "]\n");
// }

static int same_tokens(char **got, const char *exp[]) {
    size_t i = 0;
    if (!got) return 0;
    for (;; ++i) {
        const char *e = exp[i];
        const char *g = got[i];
        if (!e && !g) return 1;
        if (!e || !g) return 0;
        if (strcmp(e, g) != 0) return 0;
    }
}

// --- tests -------------------------------------------------------------------

static void test_empty_input(void) {
    char **t = lex("");
    assert(t && t[0] == NULL);
    free_tokens(t);

    t = lex("     ");
    assert(t && t[0] == NULL);
    free_tokens(t);
}

static void test_simple_split(void) {
    const char *exp1[] = {"echo", "hi", NULL};
    char **t = lex("echo hi");
    assert(same_tokens(t, exp1));
    free_tokens(t);

    const char *exp2[] = {"a", "b", "c", NULL};
    t = lex("a  b   c");
    assert(same_tokens(t, exp2));
    free_tokens(t);
}

static void test_quotes_grouping(void) {
    const char *exp1[] = {"echo", "hello world", NULL};
    char **t = lex("echo \"hello world\"");
    assert(same_tokens(t, exp1));
    free_tokens(t);

    const char *exp2[] = {"say", "a b c", "d", NULL};
    t = lex("say 'a b c' d");
    assert(same_tokens(t, exp2));
    free_tokens(t);

    const char *exp3[] = {"mix", "a\"b", "c'd", NULL};
    t = lex("mix 'a\"b' \"c'd\"");
    assert(same_tokens(t, exp3));
    free_tokens(t);
}

static void test_empty_quotes_are_tokens(void) {
    const char *exp1[] = {"", NULL};
    char **t = lex("\"\"");
    assert(same_tokens(t, exp1));
    free_tokens(t);

    const char *exp2[] = {"", "", NULL};
    t = lex("\"\" \"\"");
    assert(same_tokens(t, exp2));
    free_tokens(t);

    const char *exp3[] = {"a", "", "b", NULL};
    t = lex("a \"\" b");
    assert(same_tokens(t, exp3));
    free_tokens(t);

    const char *exp4[] = {"", NULL};
    t = lex("''");
    assert(same_tokens(t, exp4));
    free_tokens(t);
}

static void test_adjacent_quotes_in_word(void) {
    const char *exp1[] = {"ab", NULL};
    char **t = lex("a\"\"b");
    assert(same_tokens(t, exp1));
    free_tokens(t);

    const char *exp2[] = {"a\"b", NULL};
    t = lex("'a\"b'");
    assert(same_tokens(t, exp2));
    free_tokens(t);

    const char *exp3[] = {"a'b", NULL};
    t = lex("\"a'b\"");
    assert(same_tokens(t, exp3));
    free_tokens(t);
}

static void test_leading_trailing_spaces(void) {
    const char *exp1[] = {"a", "b", NULL};
    char **t = lex("   a b   ");
    assert(same_tokens(t, exp1));
    free_tokens(t);

    const char *exp2[] = {"", "x", "", NULL};
    t = lex("  \"\" x \"\"  ");
    assert(same_tokens(t, exp2));
    free_tokens(t);
}

static void test_unmatched_quotes_error(void) {
    char **t = lex("\"hello");
    assert(t == NULL);

    t = lex("abc 'def");
    assert(t == NULL);
}

static void test_redir_operators(void) {
    const char *exp1[] = {"a", "|", "b", NULL};
    char **t = lex("a|b");
    assert(same_tokens(t, exp1));
    free_tokens(t);


    const char *exp2[] = {"a", "|", "b", NULL};
    t = lex("a | b");
    assert(same_tokens(t, exp2));
    free_tokens(t);

    const char *exp3[] = {"a", "<<", "b", NULL};
    t = lex("a<<b");
    assert(same_tokens(t, exp3));
    free_tokens(t);
}

int main(void) {
    test_empty_input();
    test_simple_split();
    test_quotes_grouping();
    test_empty_quotes_are_tokens();
    test_adjacent_quotes_in_word();
    test_leading_trailing_spaces();
    test_unmatched_quotes_error();
    test_redir_operators();
    puts("OK");
    return 0;
}
