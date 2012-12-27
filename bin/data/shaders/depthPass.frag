// depth pass shader from http://ro.me/tech/

varying float depth;

void main() {
	gl_FragColor = vec4(depth, depth, depth, 1.0);
}