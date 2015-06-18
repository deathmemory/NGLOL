var gulp = require('gulp'),
	less = require('gulp-less'),//less编译
	min_css = require('gulp-minify-css'),//压缩css
	uglify = require('gulp-uglify'),//转换、压缩JS
	concat = require('gulp-concat'),//合并文件
	rename = require('gulp-rename'),//重命名
	html_min = require('gulp-htmlmin'),//压缩html
	plumber = require('gulp-plumber');//防止watch出错退出
	sourcemaps = require('gulp-sourcemaps');//源映射
	livereload = require('gulp-livereload');//刷新浏览器

// 定义工作路径 {{{
var paths = {
	js: {
		src: ['./html_ui/js/*.js','!../nikksy/www_v4/**/news.js'],
		dest: '../Debug/skin/res/html_ui/js/',
		map: '.'
	},
	html: {
		src: './html_ui/*.html',
		dest: '../Debug/skin/res/html_ui/'
	},
	less: {
		src: './html_ui/css/*.less',
		dest: '../Debug/skin/res/html_ui/css/'
	}
};

// less {{{
gulp.task('less', function() {
	gulp.src(paths.less.src)
		.pipe(plumber())
		.pipe(less())
		.pipe(min_css({compatibility:'ie7'}))
		.pipe(rename(function(path) {
			path.basename += '.min'
		}))
		.pipe(gulp.dest(paths.less.dest))
		.pipe(livereload({auto:false}));
});

// html {{{
gulp.task('html', function() {
	gulp.src(paths.html.src)
		.pipe(plumber())
        .pipe(html_min({
			removeComments:true,
			collapseWhitespace:true
		}))
		.pipe(gulp.dest(paths.html.dest))
		.pipe(livereload({auto:false}));
});

gulp.task('uglify', function() {
    gulp.src(paths.js.src)
		.pipe(plumber())
		.pipe(sourcemaps.init())
        .pipe(uglify())
        .pipe(rename(function(path) {
            if (path.basename.indexOf('load') === -1) {
                path.basename += '.min';
            }
        }))
        
		.pipe(sourcemaps.write(paths.js.map))
		.pipe(gulp.dest(paths.js.dest))
		.pipe(livereload({auto:false}));
});

// wathc{{{
gulp.task('watch', function() {
	livereload.listen();
	gulp.watch(paths.js.src, ['uglify']);
	gulp.watch(paths.less.src, ['less']);
	gulp.watch(paths.html.src, ['html']);
});

gulp.task('default', ['watch']);