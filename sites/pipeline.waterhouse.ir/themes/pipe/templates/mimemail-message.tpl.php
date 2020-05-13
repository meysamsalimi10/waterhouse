<?php

/**
 * @file
 * Default theme implementation to format an HTML mail.
 *
 * Copy this file in your default theme folder to create a custom themed mail.
 * Rename it to mimemail-message--[module]--[key].tpl.php to override it for a
 * specific mail.
 *
 * Available variables:
 * - $recipient: The recipient of the message
 * - $subject: The message subject
 * - $body: The message body
 * - $css: Internal style sheets
 * - $module: The sending module
 * - $key: The message identifier
 *
 * @see template_preprocess_mimemail_message()
 */
?>
<html>
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <?php if ($css): ?>
    <style type="text/css">
      <!--
      <?php print $css ?>
      -->
    </style>
    <?php endif; ?>
  </head>
  <body id="mimemail-body" <?php if ($module && $key): print 'class="'. $module .'-'. $key .'"'; endif; ?>>
    <div id="center">
		<div id="header-mail">
			<a href="http://waterhouse.ir"  target="_blank">
			<img src="http://waterhouse.ir/sites/default/files/iran-copy.png" alt="">
			</a>
		</div>
		<div id="main">
        <?php print $body ?>
		</div>
		<div id="footer-mail">
			<div class="first-row">
				<a href="http://waterhouse.ir"  target="_blank">با ما در ارتباط باشید</a>
			</div>
			<div>
				<div class="socialss">	
					<a class="instagram" href="http://instagram.com/iranWaterhouse" target="_blank">
						<img src="http://waterhouse.ir/sites/all/themes/waterhouse/images/socials/insta.png">
					</a>
					<a class="telegram" href="http://telegram.me/WaterOnline" target="_blank" title="عضویت در گروه تلگرام">
						<img src="http://waterhouse.ir/sites/all/themes/waterhouse/images/socials/telegram-icon.png">
					</a>
					<!--
					
					<a class="linkedin"  href="https://www.linkedin.com/company/civil808?trk=mini-profile" target="_blank">
						<img src="http://waterhouse.ir/sites/all/themes/sara/images/socials/linked.png">
					</a>
					<a class="facebook" href="https://www.facebook.com/elearning.civil" target="_blank">
						<img src="http://civil808.com/sites/all/themes/sara/images/socials/facebook.png">
					</a>
					-->
				</div>
			</div>
			<div class="las-row">
			
			</div>
		</div>
	</div>
  </body>
</html>











