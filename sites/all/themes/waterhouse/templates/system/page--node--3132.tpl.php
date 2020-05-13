<header id="navbar" role="banner" class="<?php print $navbar_classes; ?>">
    <div class="<?php print $container_class; ?>">
        <div class="navbar-header">
            <?php if ($logo): ?>
                <a class="logo navbar-btn pull-left" href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>">
                    <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
                </a>
            <?php endif; ?>

            <?php if (!empty($site_name)): ?>
                <a class="name navbar-brand" href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a>
            <?php endif; ?>

            <?php if (!empty($primary_nav) || !empty($secondary_nav) || !empty($page['navigation'])): ?>
                <button type="button" class="navbar-toggle" data-toggle="collapse" data-target=".navbar-collapse">
                    <span class="sr-only"><?php print t('Toggle navigation'); ?></span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                </button>
            <?php endif; ?>
        </div>

        <?php if (!empty($primary_nav) || !empty($secondary_nav) || !empty($page['navigation'])): ?>
            <div class="navbar-collapse collapse">
                <nav role="navigation">
                    <?php if (!empty($primary_nav)): ?>
                        <?php print render($primary_nav); ?>
                    <?php endif; ?>
                    <?php if (!empty($secondary_nav)): ?>
                        <?php print render($secondary_nav); ?>
                    <?php endif; ?>
                    <?php if (!empty($page['navigation'])): ?>
                        <?php print render($page['navigation']); ?>
                    <?php endif; ?>
                </nav>
            </div>
        <?php endif; ?>
    </div>
</header>

<div id="block-system-main">
	<section class="intro">
		<img style="width:100%" src="sites/pipeline.waterhouse.ir/files/field/files/background/site-_article-ver_2.jpg">
	</section>
	<section id="top">
	 <div class="container">
	    <div class="title">
	        <h2>ماده 10 قانون تشکیل وزارت جهاد کشاورزی (مصوب 1379)</h2>
	    </div>
	    <div class="main-text">
	        بـه منظـور هماهنگـی سياسـتگذاری در زمينـه تأميـن، توزيع و مصرف آب كشـور، شـورايی بنام شـورای عالی آب متشـكل از وزرای نيرو، جهاد كشـاورزی، صنايع و معادن، كشـور و رئيس سـازمان مديريت و برنامه ريزي كشـور، رئيس سـازمان حفاظت محيط زيسـت، (رئيـس سـازمان مديريـت بحـران كشـور)، يك نفـر از اعضـاي كميسيون كشـاورزی، آب و منابع طبيعـی با انتخاب مجلس شـوراي اسـامي بـه عنـوان ناظـر و دو نفر از متخصصين بخش كشـاورزی به حكم رئيس جمهور تشـكيل مي گردد. رياسـت عاليه اين شـورا بـا رئيس جمهـور يـا معـاون اول رياسـت جمهـوری خواهد بود. تصميمات اين شـورا بـا تصويب هيـأت وزيران يا كميسـيون مذكور در اصل یکصد و سی و هشتم (138) قانون اساسـی بـرای دسـتگاه های ذيربط لازم الاجرا مي باشـد. 


	    </div>
	 </div>
	</section>
	<style>
	#top .title{
	    text-align:-webkit-center;
	}
	#top .title h2{
    	display: table;
        background: #e7534c;
        color: white;
        border-radius: 10px;
        padding: 10px 20px;
        font-size: 18px;
        position: relative;
        top: 35px;
    }
    #top .title h2:before{
        content:'\e251';
        font-family:glyphi;
        color: #f2c438;
        position: relative;
        top: 5px;
        margin-left: 10px;

    }
    #top .main-text{
        margin-bottom: 50px;
        background: #fff;
        box-shadow: 0 0 5px 0 #ccc;
        padding: 35px 20px;
        text-align: justify;
        font-size: 16px;
        line-height: 2;
        border-top: 2px solid #e7534c;

    }
	</style>
    <script>
        /*----------------------------------*/
        $('.page-node-3132 .pdfs').each(function(){
            $(this).children('.view-content').addClass('owl-carousel');
        });
        if($('.owl-carousel').length){
            $.getScript('/sites/all/themes/waterhouse/js/lib/owl.carousel.min.js', function(){
                $('.owl-carousel').owlCarousel({
                    rtl:true,
                    loop:true,
                    margin:15,
                    responsiveClass:true,
                    nav:true,
				    autoplayTimeout:3200,
                    responsive:{
                        0:{items:1},
                        600:{items:3},
                        1000:{items:4}
                    }
                });
                $('.owl-prev').text('بعدی')
                $('.owl-next').text('قبلی')
            })
        }
    </script>
</div>

<?php if (!empty($page['footer'])): ?>
    <footer class="footer <?php print $container_class; ?>">
        <?php print render($page['footer']); ?>
    </footer>
<?php endif; ?>