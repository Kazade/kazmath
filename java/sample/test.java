
import static kazmath.jkazmath.*;

import java.nio.FloatBuffer;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class test {

	public static FloatBuffer createFloatBuffer(int size) {
		return ByteBuffer.allocateDirect(size*4).order(ByteOrder.nativeOrder()).asFloatBuffer();
	}

    public static void main(String[] args) {
        test t=new test();
        t.run();
    }
    
    // do some splurious kazmath operations to show it in use...
    
    public void run() {
        // create a mat4 struct...
        FloatBuffer mat = createFloatBuffer(16);
        
        kmMat4Identity(mat);

        // vec3 structs
        FloatBuffer eye = createFloatBuffer(3);
        FloatBuffer centre = createFloatBuffer(3);
        FloatBuffer up = createFloatBuffer(3);

        
        kmVec3Fill(eye,    0, 0, 0);
        kmVec3Fill(centre, 0, 0,-5);
        kmVec3Fill(up,     0, 1, 0);

        
        kmMat4LookAt(mat, eye, centre, up);
        kmMat4RotationPitchYawRoll(mat, 0.1f,0.2f,0.3f);
                
        // dump the end result
        System.out.println(mat.get( 0)+","+mat.get( 1)+","+mat.get( 2)+","+mat.get( 3));
        System.out.println(mat.get( 4)+","+mat.get( 5)+","+mat.get( 6)+","+mat.get( 7));
        System.out.println(mat.get( 8)+","+mat.get( 9)+","+mat.get(10)+","+mat.get(11));
        System.out.println(mat.get(12)+","+mat.get(13)+","+mat.get(14)+","+mat.get(15));
        
               
    }
    
}
