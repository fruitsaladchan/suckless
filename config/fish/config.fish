if status is-interactive
    set fish_greeting
end

alias ls='eza --icons'
alias la='eza -la --icons'
alias ll='eza -l --icons'
alias l='eza -l --icons'
alias imv='nsxiv'
alias sxiv='nsxiv'
alias cls='clear'
alias grep='rg'
alias v='nvim'
alias mm="sudo make && sudo make install"

#get pywal colors
cat ~/.cache/wal/sequences

# Start X at login
if status is-login
    if test -z "$DISPLAY" -a "$XDG_VTNR" = 1
        exec startx -- -keeptty
    end
end

# Variables
set -gx SUDO_EDITOR nvim
set -gx EDITOR nvim
set -gx PATH $HOME/.local/bin $PATH

#replace cd with zoxide
zoxide init --cmd cd fish | source
