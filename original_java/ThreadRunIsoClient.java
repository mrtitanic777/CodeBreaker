package net.minecraft.src;

class ThreadRunIsoClient extends Thread {
	final CanvasIsomPreview canvas;

	ThreadRunIsoClient(CanvasIsomPreview var1) {
		this.canvas = var1;
	}

	public void run() {
		while(CanvasIsomPreview.getRunning(this.canvas)) {
			this.canvas.render();

			try {
				Thread.sleep(1L);
			} catch (Exception var2) {
			}
		}

	}
}
