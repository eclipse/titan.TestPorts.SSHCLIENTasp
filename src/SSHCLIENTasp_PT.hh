///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Copyright Test Competence Center (TCC) ETH 2012                           //
//                                                                           //
// The copyright to the computer  program(s) herein  is the property of TCC. //
// The program(s) may be used and/or copied only with the written permission //
// of TCC or in accordance with  the terms and conditions  stipulated in the //
// agreement/contract under which the program(s) has been supplied.          //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//
//  File:               SSHCLIENTasp_PT.hh
//  Description:        SSHCLIENTasp test port header
//  Rev:                R3E
//  Prodnr:             CNL 113 484
//  Updated:            2012-03-09
//  Contact:            http://ttcn.ericsson.se
// 


#ifndef SSHCLIENTasp__PT_HH
#define SSHCLIENTasp__PT_HH

#include "SSHCLIENTasp_PortType.hh"
namespace SSHCLIENTasp__PortType {

typedef struct {
    boolean match;
    unsigned int start;
    unsigned int end;
} Regex_Prompt_MatchResult_SSH_client;

class Regex_Prompt_SSH_client {
    regex_t posix_regexp;
public:
    Regex_Prompt_SSH_client(const char *p_pattern, boolean raw_prompt = false);
    ~Regex_Prompt_SSH_client();
    Regex_Prompt_MatchResult_SSH_client match(const char *msg);
};

class Prompt_List_SSH_client {
    typedef struct {
    unsigned int id;
    boolean is_regex;
    union {
        char *prompt;
        Regex_Prompt_SSH_client *regex_prompt;
    };
    } prompt_elem;
    prompt_elem **elems;
    size_t n_elems;
    boolean id_to_index(unsigned int p_id, size_t &index) const;
public:
    Prompt_List_SSH_client() : elems(NULL), n_elems(0) { }
    ~Prompt_List_SSH_client();
    void set_prompt(unsigned int p_id, const char *p_prompt, boolean p_is_regex, boolean p_is_raw = false);
    void check(const char *warning_prefix);
    void clear();
    size_t nof_prompts() const { return n_elems; }
    int findPrompt(size_t &pos, const unsigned char *bufptr, size_t buflen) const;
};


class SSHCLIENTasp__PT : public SSHCLIENTasp__PT_BASE {
public:
    SSHCLIENTasp__PT(const char *par_port_name = NULL);
    ~SSHCLIENTasp__PT();

    void set_parameter(const char *parameter_name,
        const char *parameter_value);

    void Event_Handler(const fd_set *read_fds,
        const fd_set *write_fds, const fd_set *error_fds,
        double time_since_last_call);

protected:
    void user_map(const char *system_port);
    void user_unmap(const char *system_port);

    void user_start();
    void user_stop();

    void log(const char *fmt, ...)
           __attribute__((format(printf, 2, 3)));
    void error(const char *fmt, ...)
           __attribute__((format(printf, 2, 3)));
    void cleanup();
    void outgoing_send(const SSHCLIENTasp__Types::ASP__SSH& send_par);
    void outgoing_send(const SSHCLIENTasp__Types::ASP__SSH__SetPrompt& send_par);
    void outgoing_send(const SSHCLIENTasp__Types::ASP__SSH__SetRegexPrompt& send_par);
    void outgoing_send(const SSHCLIENTasp__Types::ASP__SSH__ClearPrompt& send_par);
    void outgoing_send(const SSHCLIENTasp__Types::ASP__SSH__SetMode& send_par);
    void outgoing_send(const SSHCLIENTasp__Types::ASP__SSH__Connect& send_par);
    void outgoing_send(const SSHCLIENTasp__Types::ASP__SSH__SetUserID& send_par);
    void outgoing_send(const SSHCLIENTasp__Types::ASP__SSH__SetRemoteHost& send_par);
    void outgoing_send(const SSHCLIENTasp__Types::ASP__SSH__SetRemotePort& send_par);
    void outgoing_send(const SSHCLIENTasp__Types::ASP__SSH__SetAdditionalParameters& send_par);
    int         isPrompt(size_t& pos);
    int         RecvMsg();
    void        log_buffer(const char * logmsg,
                    const unsigned char * buf, size_t buflen);
    boolean     buf_strcmp(const char * s1, const unsigned char * s2,
                size_t s2_len, size_t& pos);
    boolean     find_echo(const char * s1, const unsigned char * s2,
                size_t s2_len, size_t& pos); 
    void        addPrompt(const char* parameter_name, const char* parameter_value);
    void        addRegexPrompt(const char* parameter_name, const char* parameter_value);

private:
    int fd_ssh;
    pid_t pid;

    //char recbuf[8192];

    int num_of_params;
    char **additional_parameters;
    /* The additional_parameters stores the parameters of the ssh
      index      meaning
      0         "ssh"
      1         ip_version
      2         userID
      3         remote_port
      4         remote_host
      ...       additional parameters
      last      NULL
    */
    CHARSTRING EOL;
    CHARSTRING LastSent;

    boolean assignEOL;
    boolean supressEcho;
    boolean msgrecv;
    boolean supressPrompt;
    boolean pseudoPrompt;
    boolean detectServerDisconnected;
    boolean debug;
    boolean statusOnSuccess;
    boolean emptyEcho;
    boolean suppressed;
    boolean rawPrompt;
       int  readmode;
    CHARSTRING    echobuf;
    TTCN_Buffer ttcn_buf;
    fd_set readfds;
    Prompt_List_SSH_client prompt_list;
    
    char **add_params(int &num_of_par, char **params, const char *new_params);
};
}
#endif
