/*
  oo_cmdbreech - breech-loading command processor

  (C)Copyright 2015 Smithee,Spelvin,Agnew & Plinge, Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/


#include <string.h>


#include <jansson.h>
#include <gnutls/gnutls.h>


#include <osdp-tls.h>
#include <open-osdp.h>


extern OSDP_OUT_CMD
  current_output_command [];


int
  read_command
    (OSDP_CONTEXT
      *ctx,
    OSDP_COMMAND
      *cmd)

{ /* read_command */

  FILE
    *cmdf;
  char
    field [1024];
  char
    json_string [16384];
  json_t
    *root;
  json_error_t
    status_json;
  int
    status;
  int
    status_io;
  char
    *test_command;
  char
    this_command [1024];
  json_t
    *value;


  status = ST_CMD_PATH;
  cmdf = fopen (ctx->command_path, "r");
  if (cmdf != NULL)
  {
    status = ST_OK;
    memset (json_string, 0, sizeof (json_string));
    status_io = fread (json_string,
      sizeof (json_string [0]), sizeof (json_string), cmdf);
    if (status_io >= sizeof (json_string))
      status = ST_CMD_OVERFLOW;
    if (status_io <= 0)
      status = ST_CMD_UNDERFLOW;
  };

  if (status EQUALS ST_OK)
  {
    root = json_loads (json_string, 0, &status_json);
    if (!root)
    {
      fprintf (stderr, "JSON parser failed.  String was ->\n%s<-\n",
        json_string);
      status = ST_CMD_ERROR;
    };
    if (status EQUALS ST_OK)
    {
      strcpy (field, "command");
      value = json_object_get (root, field);
      if (!json_is_string (value))
        status = ST_CMD_INVALID;
    };
  };
  if (status EQUALS ST_OK)
  {
    strcpy (this_command, json_string_value (value));
    test_command = "capabilities";
    if (0 EQUALS strncmp (this_command, test_command, strlen (test_command)))
    {
      cmd->command = OSDP_CMDB_CAPAS;
      if (ctx->verbosity > 4)
        fprintf (stderr, "command was %s\n",
          this_command);
    };
  }; 
  if (status EQUALS ST_OK)
  {
    strcpy (this_command, json_string_value (value));
    test_command = "dump_status";
    if (0 EQUALS strncmp (this_command, test_command, strlen (test_command)))
    {
      cmd->command = OSDP_CMDB_DUMP_STATUS;
      if (ctx->verbosity > 3)
        fprintf (stderr, "command was %s\n",
          this_command);
    };
  }; 
  if (status EQUALS ST_OK)
  {
    strcpy (this_command, json_string_value (value));
    test_command = "identify";
    if (0 EQUALS strncmp (this_command, test_command, strlen (test_command)))
    {
      cmd->command = OSDP_CMDB_IDENT;
      if (ctx->verbosity > 3)
        fprintf (stderr, "command was %s\n",
          this_command);
    };
  }; 
  if (status EQUALS ST_OK)
  {
    int
      i;
    char
      vstr [1024];

    strcpy (this_command, json_string_value (value));
    test_command = "output";
    if (0 EQUALS strncmp (this_command, test_command, strlen (test_command)))
    {
      cmd->command = OSDP_CMDB_OUT;
      if (ctx->verbosity > 3)
        fprintf (stderr, "command was %s\n",
          this_command);

      // default values in case some are missing

      current_output_command [0].output_number = 0;
      current_output_command [0].control_code = 2; // permanent on immediate
      current_output_command [0].timer = 0; // forever

      // the output command takes arguments: output_number, control_code

      value = json_object_get (root, "output_number");
      if (json_is_string (value))
      {
        strcpy (vstr, json_string_value (value));
        sscanf (vstr, "%d", &i);
        current_output_command [0].output_number = i;
      };
      value = json_object_get (root, "control_code");
      if (json_is_string (value))
      {
        strcpy (vstr, json_string_value (value));
        sscanf (vstr, "%d", &i);
        current_output_command [0].control_code = i;
      };
      value = json_object_get (root, "timer");
      if (json_is_string (value))
      {
        strcpy (vstr, json_string_value (value));
        sscanf (vstr, "%d", &i);
        current_output_command [0].timer = i;
      };
    };
  }; 
  if (status EQUALS ST_OK)
  {
    value = json_object_get (root, "command");

    strcpy (this_command, json_string_value (value));
    test_command = "present_card";
    if (0 EQUALS strncmp (this_command, test_command, strlen (test_command)))
    {
      cmd->command = OSDP_CMDB_PRESENT_CARD;
      if (ctx->verbosity > 3)
        fprintf (stderr, "command was %s\n",
          this_command);
    };
  }; 
  if (status EQUALS ST_OK)
  {
    strcpy (this_command, json_string_value (value));
    test_command = "reset_power";
    if (0 EQUALS strncmp (this_command, test_command, strlen (test_command)))
    {
      cmd->command = OSDP_CMDB_RESET_POWER;
      if (ctx->verbosity > 3)
        fprintf (stderr, "command was %s\n",
          this_command);
    };
  }; 
  if (status EQUALS ST_OK)
  {
    strcpy (this_command, json_string_value (value));
    test_command = "send_poll";
    if (0 EQUALS strncmp (this_command, test_command, strlen (test_command)))
    {
      cmd->command = OSDP_CMDB_SEND_POLL;
      if (ctx->verbosity > 3)
        fprintf (stderr, "command was %s\n",
          this_command);
    };
  }; 
  if (cmdf != NULL)
    fclose (cmdf);
  return (status);

} /* read_command */

