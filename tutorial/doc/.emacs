;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; add the user's local emacs directory to the
;; set of dirs searched when loading a library
(setq load-path (cons "~/Emacs" load-path))

(setq check-then-require-verbose nil)	; Set to 't' to see which elisp code is unavail
(defun check-then-require (arg)
  "Checks if ARG exists in load-path before requiring"
  (if (path-filep load-path arg) 
      (require arg)
    (if check-then-require-verbose (error "Cannot find load file: %s" arg))))

(defun path-filep (path arg)
  "Checks if ARG exists in PATH"
  (if (not path) nil
    (if (or (file-exists-p (concat (car path) "/" (format "%s" arg) ".el"))
	    (file-exists-p (concat (car path) "/" (format "%s" arg) ".elc"))
	    (path-filep (cdr path) arg)) t)))

;(setq debug-on-error t)
;(setq stack-trace-on-error t)		; Enable for debugging

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Global defaults and key bindings
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;; add my own key binding here James 990525

(global-set-key [M-kp-multiply] 'set-mark-command)
(global-set-key [M-f26] 'set-mark-command)
(global-set-key [M-kp-enter] 'exchange-point-and-mark)
(global-set-key [M-f24] 'clipboard-kill-region)
(global-set-key [M-kp-subtract] 'clipboard-kill-region)
(global-set-key [M-kp-add] 'clipboard-kill-ring-save)
(global-set-key [M-f25] 'clipboard-yank)
(global-set-key [M-kp-divide] 'clipboard-yank)

(global-set-key [C-delete] 'kill-line)
(global-set-key [C-backspace] 'kill-line)
(global-set-key [C-BS] 'kill-line)
(global-set-key [M-home] 'beginning-of-buffer)
(global-set-key [M-end] 'end-of-buffer)
(global-set-key [home] 'beginning-of-line)
(global-set-key [end] 'end-of-line)
(global-set-key [f27] 'beginning-of-line)
(global-set-key [f33] 'end-of-line)
(global-set-key [f35] 'scroll-up)
(global-set-key [f29] 'scroll-down)
(global-set-key [C-home] 'beginning-of-buffer)
(global-set-key [C-f27] 'beginning-of-buffer)
(global-set-key [C-end] 'end-of-buffer)
(global-set-key [C-f33] 'end-of-buffer)

(global-set-key "\M-a" 'shell)
(global-set-key "\M-b" 'keyboard-quit)
(global-set-key "\M-e" 'make-frame)
(global-set-key "\M-n" 'yank-rectangle)
(global-set-key "\M-m" 'kill-rectangle)
(global-set-key "\M-c" 'kill-ring-save)
(global-set-key "\M-d" 'dired)
(global-set-key "\M-f" 'find-file)
(global-set-key "\M-g" 'goto-line)
(global-set-key "\M-j" 'fill-paragraph)
(global-set-key "\M-k" 'kill-buffer)
(global-set-key "\M-l" 'buffer-menu)
(global-set-key "\M-o" 'other-window)
(global-set-key "\M-p" 'print-buffer)
(global-set-key "\C-p" 'print-region)
(global-set-key "\M-q" 'query-replace)
(global-set-key "\M-r" 'revert-buffer)
(global-set-key "\M-s" 'save-buffer)
(global-set-key "\M-t" 'ispell-buffer)
(global-set-key "\M-u" 'undo)
(global-set-key "\M-v" 'yank)
(global-set-key "\M-x" 'kill-region)
(global-set-key "\M-w" 'write-file)

;; frame title is the buffer name
(setq frame-title-format     "%b")

(global-set-key [M-up] 'set-mark-command )
(global-set-key [M-right] 'forward-word )
(global-set-key [M-left] 'backward-word )
(global-set-key [M-down] 'forward-paragraph )

(global-set-key "\M-1" 'delete-other-windows)
(global-set-key "\M-2" 'split-window-vertically)
(global-set-key "\M-3" 'split-window-horizontally)
(global-set-key [f11] 'keyboard-quit)

; their original bindings, I do not like this
;(global-set-key "\C-xd" 'delete-window)	; "Ctrl-x 0" does the same
;(global-set-key "\C-xs" 'save-buffer)	; overrides previous binding
;(global-set-key "\C-xc" 'center-line)	; center is now Ctrl-X c
;(global-set-key "\C-x?" 'describe-key)
;(global-set-key "\C-?" 'help-command)
;(global-set-key "\C-^" '(lambda ()	; primitive redo
;			  (interactive) (undo-start)))
; help is now ESC h
;(global-set-key "\M-h" 'help-for-help)
;(global-set-key "\C-h" 'delete-backward-char)
;(global-set-key "\C-?" 'delete-backward-char)
;(global-set-key "\C-z" '(lambda () (interactive) (scroll-up 1)))
;(global-set-key "\M-\C-z" 'suspend-emacs)
;(global-set-key "\C-c\C-m" 'compile)
;(global-set-key "\C-c\C-h" 'simulate)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; bindings for GDB so that they work in non-gdb
;;  windows when such bindings don't already exist
;;  in those windows

;(global-set-key "\C-c\C-g" 'gdb)
(global-set-key "\C-c>"	'gud-down)
(global-set-key "\C-c<"	'gud-up)
(global-set-key "\C-c\C-f" 'gud-finish)
;(global-set-key "\C-c\C-s" 'gud-step)
(global-set-key "\C-c\C-t" 'gud-tbreak)
;(global-set-key "\C-c\C-b" 'gud-break)
(global-set-key "\C-c\C-d" 'gud-remove)
;(global-set-key "\C-c\C-p" 'gud-print)
(global-set-key "\C-c\C-n" 'gud-next)
(global-set-key "\C-c\C-l" 'gud-refresh)
(global-set-key "\C-c\C-r" 'gud-cont)

(global-set-key "\C-xj"
		'(lambda (char)		; Rebind to set mark
		   (interactive "cJump to register: ")
		   (let ((val (get-register char)))
		     (if (null val)
			 (error "Register %s is empty"
				(single-key-description char)))
		     (and (stringp val)
			  (error "Register %s contains text"
				 (single-key-description char)))
		     (if (eq (marker-buffer val)
				  (current-buffer))
			 (progn (push-mark) ; Save point w/in same buf
				;(set-mark (point)) ; Not as flexible
				(register-to-point char))))))
(global-set-key "\C-xy"
		'(lambda (char)			  
		   (interactive "cRead from register: ")
		   (let ((val (get-register char)))
		     (if (null val)
			 (error "Register %s is empty"
				(single-key-description char)))
		     (or (stringp val)
			 (error "Register %s does not contain text")
			 (single-key-description char))
		     (insert (get-register char))
		     (message "Read from register %s"
			      (single-key-description char)))))

(setq-default apropos-do-all t)		; apropos & super-apropos should do everything
(setq-default compile-command "make")	; default is "make -k"
(setq-default require-final-newline t)
(setq-default next-line-add-newlines nil)
(setq-default truncate-lines nil)
;(setq-default visible-bell t)		; for ascii terminals, do not ring bell
(setq-default tags-file-name "TAGS")
(setq-default default-fill-column 80)
(setq-default inhibit-startup-message t) ; start up with a clean slate
(setq-default inhibit-startup-echo-area-message t)

; Show all buffers in the 'Buffers' menu:
(setq-default buffers-menu-max-size nil)

;(setq-default make-backup-files nil)	; Disables backup file creation

;;; Does the following line work?
(setq-default transient-mark-mode nil)	; Disable mark deactivation upon change in buf

(cond ((string-match "^18\." emacs-version)

       ;; put Version 18 only code here
       (global-set-key "\C-xn" 'other-window)
       (global-set-key "\C-xp" 'other-window)
       (cond (window-system
	      (x-set-bell 'visible)))
       )
      ((string-match "^19\." emacs-version)
       ;; put Version 19 only code here..
       (global-set-key "\C-xn" 'next-multiframe-window)
       (global-set-key "\C-xp" 'previous-multiframe-window)
       (define-key global-map [C-right] 'forward-sexp)
       (define-key global-map [C-left] 'backward-sexp)
       (define-key global-map [C-up] 'backward-list)
       (define-key global-map [C-down] 'forward-list)
       (setq-default line-number-mode t)
       ;; Local stuff..
       (if (check-then-require 'spice-mode) ; Not full-proof,  hilit-install-line-hooks
	   (setq auto-mode-alist
		 (append (list (cons "\\.sp$" 'spice-mode)) auto-mode-alist)))
       (if (check-then-require 'pisces-mode)
	   (setq auto-mode-alist
		 (append (list (cons "\\.pis$" 'pisces-mode)) auto-mode-alist)))
       (check-then-require 'simulate)
       (setq crypt-freeze-vs-fortran nil)
       (check-then-require 'crypt++)  ;; Transparent viewing & editing of compressed files
       (add-hook 'text-mode-hook
		 '(lambda ()
		    (auto-fill-mode t)		; Or (turn-on-auto-fill)
		    (define-key text-mode-map "\M-s" 'save-buffer)))
       (add-hook 'c-mode-hook
		 '(lambda ()
		    (define-key c-mode-map "\C-c;" 'comment-region)))
       (add-hook 'c++-mode-hook
		 '(lambda ()
		    (define-key c++-mode-map "\C-c;" 'comment-region)))
       (add-hook 'fortran-mode-hook 'my-fortran-hook t)
       (add-hook 'gdb-mode-hook 
		 '(lambda ()
		    (gud-def gud-until
		     "until" "\C-u" "Step one line (skip functions).")))
       (add-hook 'emacs-lisp-mode-hook
		 '(lambda ()
		    (define-key emacs-lisp-mode-map "\C-c;" 'comment-region)))

))

(defun my-fortran-hook ()
  "add bindings to fortran-mode-map"
  (define-key fortran-mode-map [C-up]	; Rebind to set mark
    '(lambda () (interactive) 
       ; (set-mark (point))		; Not as flexible
       (push-mark)
       (beginning-of-fortran-subprogram)))
  (define-key fortran-mode-map [C-down]	; Rebind to set mark
    '(lambda () (interactive) 
       ; (set-mark (point))		; Not as flexible
       (push-mark)
       (message "Mark set")
       (end-of-fortran-subprogram)))
  (setq fortran-continuation-string "+")
  (setq fortran-comment-region "c...")
  (setq fortran-blink-matching-if t)
  (define-key fortran-mode-map "\C-c;"
    '(lambda (beg-region end-region)
       "Comment wrapper"
       (interactive "*r")
       (let ((save-point (point-marker)))
         (goto-char beg-region)
         (if (looking-at fortran-comment-region)
             (progn (goto-char save-point)
                    (fortran-comment-region beg-region end-region 1))
           (goto-char save-point)
           (fortran-comment-region beg-region end-region nil))))))


; Strips ^M in shell window, doesn't seem to be necessary
;(add-hook 'shell-mode-hook (function (lambda ()
;                            (add-hook 'comint-output-filter-functions
;                                      'shell-strip-ctrl-m))))

;; (setq default-major-mode 'text-mode)  ;; I do not like text mode

(autoload 'matlab-mode "matlab-mode" "Enter Matlab mode." t)
(defun my-matlab-mode-hook ()
  (setq fill-column 76)
  (turn-on-auto-fill))
(setq matlab-mode-hook 'my-matlab-mode-hook)
(setq auto-mode-alist
      (nconc '(
               ("\\.v$"               . c++-mode)
              ) auto-mode-alist))


(setq auto-mode-alist
      (append (list (cons "\\.tcl$" 'tcl-mode))
	      (list (cons "\\.pl$" 'perl-mode))
	      (list (cons "\\.ms$" 'nroff-mode))
	      (list (cons "\\.awk$" 'c-mode))
	      (list (cons "\\.m$" 'matlab-mode)) ; Requires matlab-mode.el
	      (list (cons "[Mm]akefile" 'makefile-mode))
	      (list (cons "\\.mk$" 'makefile-mode))
	      (list (cons "\\.F\\|fh$" 'fortran-mode))
	      (list (cons "\\.v$"  'c++-mode))
	      (list (cons "\\.tdl$"  'c++-mode))
	      auto-mode-alist))
(and (boundp 'interpreter-mode-alist)	; modify const alist
     (setq interpreter-mode-alist (append (list (cons "expect" 'tcl-mode))
	interpreter-mode-alist)))

;; Makes the % key show the matching parenthesis, like in vi if the cursor
;;  isn't over a parenthesis

    (global-set-key "%" 'match-paren)
    (defun match-paren (arg)
      "Go to the matching parenthesis if on parenthesis otherwise insert %."
      (interactive "p")
      (cond ((looking-at "\\s\(") (forward-list 1) (backward-char 1))
	    ((looking-at "\\s\)") (forward-char 1) (backward-list 1))
	    (t (self-insert-command (or arg 1)))))
  
(put 'eval-expression 'disabled nil)	; <Esc>-<Esc> as elisp-eval

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Startup commands
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; I do not want time
;(display-time)

;; For displaying european character sets
;(load-library "iso-insert")
; For inputting big5 characters
;(set-input-mode (car (current-input-mode)) (nth 1 (current-input-mode)) 0)
;(standard-display-european t)
; (global-set-key   [C-f10]      'insert-u-umlaut)
;; Already bound to C-x 8 \" u

;; For WWW on ascii terminals
;(setq load-path (cons (expand-file-name "~/lib/w3") load-path))
;(autoload 'w3-preview-this-buffer "w3" "WWW Previewer" t)
;(autoload 'w3-follow-url-at-point "w3" "Find document at pt" t)
;(autoload 'w3 "w3" "WWW Browser" t)
;(autoload 'w3-open-local "w3" "Open local file for WWW browsing" t)
;(autoload 'w3-fetch "w3" "Open remote file for WWW browsing" t)
;(autoload 'w3-use-hotlist "w3" "Use shortcuts to view WWW docs" t)
;(autoload 'gopher-dispatch-object "gopher" "Fetch gopher dir" t)
;(autoload 'html-mode "html-mode" "Edit HTML docs" t)

;(if (and (string-match "Emacs 19" (emacs-version)) (eq window-system 'x))
;  (require 'hilit19))

(cond ((string-match "^19\." emacs-version)
;       (load "sh-script")			; lacks (provide '<mode>)!
;       (setq sh-find-file-modifies nil)
(cond (window-system

       (set-face-background 'region "yellow") ; Sets the colors of mouse-drag area
       (set-face-foreground 'region "black") ; Sets the colors of mouse-drag area
       (set-face-background 'modeline "white")
       (set-face-foreground 'modeline "black")
       (set-face-background 'highlight "green") ; Sets colors on completions
       (set-face-foreground 'highlight "black") ; Sets colors on completions

       ;; Enables ability to drag modeline, scrollbars, local?
       (global-set-key [mode-line down-mouse-1] 'mldrag-drag-mode-line)
       (global-set-key [vertical-line down-mouse-1] 'mldrag-drag-vertical-line)
       (global-set-key [vertical-scroll-bar S-down-mouse-1]
		       'mldrag-drag-vertical-line)
       (check-then-require 'mldrag)

       (setq hilit-mode-enable-list  '(not text-mode)
	     hilit-background-mode   'dark
	     hilit-inhibit-hooks     nil
	     hilit-inhibit-rebinding nil)
       (setq hilit-user-face-table
	     ;; face         light            dark            mono
	     '((type         nil              nil             nil)
;	       (jargon-entry defun            blue            bold)
;	       (comment      firebrick-italic blue          italic)
;	       (include      purple           Plum1           bold-italic)
;	       (define       ForestGreen-bold blue            bold)
;	       (defun        blue-bold        cyan            bold-italic)
;	       (decl         RoyalBlue        cyan            bold)
;	       (type         nil              red             nil)
;	       (keyword      RoyalBlue        blue            bold-italic)
;	       (label        red-underline    red             underline)
;	       (string       grey             red             underline)
	       ))
; a hopefully more informative message
       (add-hook 'emacs-startup-hook '(lambda () (message (emacs-version))))
       (if (not (check-then-require 'hl319)) ; Preferable!
	   (require 'hilit19)
       ; For highlighting on the fly, unavailable in hilit19!
	 (add-hook 'c-mode-hook '(lambda () (hilit-install-line-hooks)))
	 (add-hook 'emacs-lisp-mode-hook
		   '(lambda () (hilit-install-line-hooks)))
	 (add-hook 'fortran-mode-hook '(lambda () (hilit-install-line-hooks)))
	 (add-hook 'awk-mode-hook '(lambda () (hilit-install-line-hooks)))
	 (add-hook 'perl-mode-hook '(lambda () (hilit-install-line-hooks)))
	 (add-hook 'matlab-mode-hook '(lambda () (hilit-install-line-hooks)))
	 (add-hook 'makefile-mode-hook '(lambda () (hilit-install-line-hooks)))
	 (add-hook 'lisp-mode-hook '(lambda () (hilit-install-line-hooks)))
	 (add-hook 'tar-mode-hook '(lambda () (hilit-install-line-hooks)))
	 (add-hook 'spice-mode-hook '(lambda () (hilit-install-line-hooks)))
	 (add-hook 'pisces-mode-hook '(lambda () (hilit-install-line-hooks)))
	 (add-hook 'nroff-mode-hook '(lambda () (hilit-install-line-hooks)))
	 (add-hook 'c++-mode-hook '(lambda () (hilit-install-line-hooks)))
	 (add-hook 'simulation-mode-hook 
		   '(lambda () (hilit-install-line-hooks))))
       (hilit-set-mode-patterns
	'matlab-mode
	'(("%.*$" nil comment)
	  ("'.*'" nil string)
	  ("^[ \t]*function.*\\.\\.\\.+[ \t]*\\(%.*\\)?\\([ \t\n]*.*\\.\\.\\.+[ \t]*\\(%.*\\)?\\)*[\n].*$" nil defun)
	  ;; key words
	  ("\\<\\(load\\|save\\|clear\\)\\>" nil include)
	  ("\\<\\(if\\|elseif\\|else\\|end\\|for\\|while\\|break\\|return\\|pause\\)\\>" nil keyword)
	  ))
       (hilit-set-mode-patterns
	'pisces-mode
	'(("^\$.*$" nil comment)
	  ("^solve.*$" nil keyword))
	)
       (hilit-set-mode-patterns
	'simulation-mode
	'(("^>warning.*$" nil warning)
	  ("^[ \t]*\\*\\*warning.*$" nil warning)
	  ("^\>error.*$" nil error)
	  ("^[ \t]*\\*\\*error.*$" nil error)
	  ("^>.*$" nil urgent)
	  ("^.*\a.*$" nil urgent))
	)
       (hilit-set-mode-patterns
	'spice-mode
	'(("^\*.*$" nil comment)
	  ("^\\.inc.*$" nil include)
	  ("^\\.[^ \t]+" nil keyword)
	  ("\$.*$" nil comment)
;	  ("\\`.*$" nil comment)		; Title card, causes loop
	))
       (setq hilit-auto-highlight-maxout 100000000) ; Increase the line # limit
       ))
))

; Local stuff...

(if (and (boundp 'doc-directory) (boundp 'internal-doc-file-name)
	 (file-exists-p (concat doc-directory internal-doc-file-name))) nil
	 (setq-default doc-directory "/u1/wjhuang/Emacs/lib/etc/")
	 (setq-default internal-doc-file-name "DOC-19.25.1"))

(if (file-exists-p ".treat_h_as_fh")	; Primitive hack, checks cwd
    (setq auto-mode-alist (append (list (cons "\\.h$" 'fortran-mode))
                                auto-mode-alist)))


(put 'set-goal-column 'disabled nil)



; Mathematica emacs mode 

(put 'eval-expression 'disabled nil)
;; 
;; Create your own emacs directory to use on non-Sweet Hall machines
;;
(setq load-path (append load-path (list "/usr/class/cs050/emacs")))
(autoload 'math "math" "Starts Mathematica" t)
(autoload 'math-mode "math"
   "Mode for editing Mathematica.  Loading will result in more info." t)
;;
;; To avoid confusion with Notebook format and conflicts with other
;; languages, use .math extension
;;
(setq auto-mode-alist (cons '("\.math$" . math-mode) auto-mode-alist))


;;
;; EE282H emacs
;; 
;;

; for emacs-19

; for highlighting
(cond (window-system
       (progn
         (setq hilit-mode-enable-list  '(not text-mode)
               hilit-background-mode   'dark
               hilit-inhibit-hooks     nil
               hilit-inhibit-rebinding nil)

         (require 'hilit19)

;         (menu-bar-mode nil)
         (scroll-bar-mode t)
         (line-number-mode t)
;         (server-start)
        )))


;;
;; show line and col #
;; James 990525
(line-number-mode 1)
(column-number-mode 1)
(transient-mark-mode 1)
(setq kill-whole-line 1)
(setq line-number-display-limit 100000000)

;; for matlab
(autoload 'matlab-mode "matlab" "Enter Matlab mode." t)
(setq auto-mode-alist (cons '("\\.m\\'" . matlab-mode) auto-mode-alist))
(add-hook 'matlab-mode-hook '(lambda () (font-lock-mode 1)))

;; show buffer name on title

