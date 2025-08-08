# include <stdio.h>
# include <string.h>
# include <stdlib.h>

int confirm_action(const char *action) {
    char cmd[128];
    snprintf(cmd, sizeof(cmd),
        "printf 'Yes\\nNo' | dmenu -i -p '%s?'", action);
    FILE *confirm = popen(cmd, "r");
    if (!confirm) return 0;

    char response[8];
    int confirmed = 0;

    if (fgets(response, sizeof(response), confirm)) {
        if (strncmp(response, "Yes", 3) == 0)
            confirmed = 1;
    }

    pclose(confirm);
    return confirmed;
}

void exitdwm ()
{
# if							   \
	defined S_LOCK				|| \
	defined S_RESTART_DWM		|| \
	defined S_OFFSCREEN			|| \
	defined S_EXIT				|| \
	defined S_REBOOT			|| \
	defined S_SHUTDOWN			|| \
	defined S_KILL_PROCESS		|| \
	defined S_LOCK_ICON			|| \
	defined S_RESTART_DWM_ICON	|| \
	defined S_OFFSCREEN_ICON	|| \
	defined S_EXIT_ICON			|| \
	defined S_REBOOT_ICON		|| \
	defined S_SHUTDOWN_ICON		|| \
	defined S_KILL_PROCESS_ICON	|| \
	defined S_FORMAT			|| \
	defined S_FORMAT_CLEAR
# error (conflicting macro names)
# endif

# define S_LOCK "Lock"
# define S_RESTART_DWM "restart Dwm"
# define S_OFFSCREEN "Off-screen"
# define S_EXIT "Exit"
# define S_REBOOT "Reboot"
# define S_SHUTDOWN "Shutdown"
# define S_KILL_PROCESS "kill Process"

# define S_LOCK_ICON "\uf023"
# define S_RESTART_DWM_ICON "\uf01e"
# define S_OFFSCREEN_ICON "\uf108"
# define S_EXIT_ICON "\uf2f5"
# define S_REBOOT_ICON "\uf021"
# define S_SHUTDOWN_ICON "\uf011"
# define S_KILL_PROCESS_ICON "\xE2\x9C\x96"

# define S_FORMAT(ACTION) S_##ACTION##_ICON " " S_##ACTION
# define S_FORMAT_CLEAR "sed 's/^..//'"

	FILE * exit_menu = popen (
		"echo \""
            S_FORMAT (KILL_PROCESS) "\n"
			S_FORMAT (LOCK) "\n"
			S_FORMAT (RESTART_DWM) "\n"
			S_FORMAT (OFFSCREEN) "\n"
			S_FORMAT (EXIT) "\n"
			S_FORMAT (REBOOT) "\n"
			S_FORMAT (SHUTDOWN) "\n"
			"\" | dmenu -i | " S_FORMAT_CLEAR,
		"r"
	);

	char exit_action[32];

	if (
		exit_menu == NULL ||
		fscanf(exit_menu, "%31[a-zA-Z -]", exit_action) == EOF
	) {
		fputs("Error. Failure in exit_dwm.", stderr);
		goto close_streams;
	}

	if (strcmp(exit_action, S_LOCK) == 0)
		system("slock & sleep .5; xset dpms force off");
	else if (strcmp(exit_action, S_RESTART_DWM) == 0)
		quit(&(const Arg) {1});
	else if (strcmp(exit_action, S_OFFSCREEN) == 0)
		system("sleep .5; xset dpms force off");
	else if (strcmp(exit_action, S_EXIT) == 0)
		quit(&(const Arg) {0});
	else if (strcmp(exit_action, S_REBOOT) == 0) {
		if (confirm_action("Reboot"))
			system("systemctl reboot");
	}
	else if (strcmp(exit_action, S_SHUTDOWN) == 0) {
		if (confirm_action("Shutdown"))
			system("systemctl poweroff -i");
	}
	else if (strcmp(exit_action, S_KILL_PROCESS) == 0) {
		FILE *proc_menu = popen(
			"ps -eo pid,comm,%cpu,%mem --sort=-%cpu | "
			"dmenu -i -l 20 -p \"kill PID:\"",
			"r"
		);

		if (proc_menu != NULL) {
			char line[128];
			if (fgets(line, sizeof(line), proc_menu)) {
				int pid = 0;
				sscanf(line, "%d", &pid);
				if (pid > 0) {
					char cmd[64];
					snprintf(cmd, sizeof(cmd), "kill -9 %d", pid);
					system(cmd);
				}
			}
			pclose(proc_menu);
		}
	}

close_streams:
	pclose(exit_menu);

# undef S_LOCK
# undef S_RESTART_DWM
# undef S_OFFSCREEN
# undef S_EXIT
# undef S_REBOOT
# undef S_SHUTDOWN
# undef S_KILL_PROCESS
# undef S_LOCK_ICON
# undef S_RESTART_DWM_ICON
# undef S_OFFSCREEN_ICON
# undef S_EXIT_ICON
# undef S_REBOOT_ICON
# undef S_SHUTDOWN_ICON
# undef S_KILL_PROCESS_ICON
# undef S_FORMAT
# undef S_FORMAT_CLEAR
}

