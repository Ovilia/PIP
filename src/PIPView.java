/*
 * PIPView.java
 */
package pip;

import org.jdesktop.application.Action;
import org.jdesktop.application.ResourceMap;
import org.jdesktop.application.SingleFrameApplication;
import org.jdesktop.application.FrameView;
import org.jdesktop.application.Task;
import org.jdesktop.application.TaskMonitor;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.image.BufferedImage;
import java.io.File;
import javax.swing.GroupLayout;
import javax.swing.Timer;
import javax.swing.Icon;
import javax.swing.JDialog;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;

/**
 * The application's main frame.
 */
public class PIPView extends FrameView {

    public PIPView(SingleFrameApplication app) {
        super(app);

        initComponents();

        tabbedPane = new JTabbedPane();
        javax.swing.GroupLayout mainPanelLayout = new javax.swing.GroupLayout(mainPanel);
        mainPanel.setLayout(mainPanelLayout);
        mainPanelLayout.setHorizontalGroup(
                mainPanelLayout.createParallelGroup(
                javax.swing.GroupLayout.Alignment.LEADING).addComponent(jToolBar, javax.swing.GroupLayout.DEFAULT_SIZE,
                753, Short.MAX_VALUE).addGroup(mainPanelLayout.createSequentialGroup().addGap(10, 10, 10).addComponent(tabbedPane,
                javax.swing.GroupLayout.DEFAULT_SIZE, 733, Short.MAX_VALUE).addContainerGap()));
        mainPanelLayout.setVerticalGroup(
                mainPanelLayout.createParallelGroup(
                javax.swing.GroupLayout.Alignment.LEADING).addGroup(mainPanelLayout.createSequentialGroup().addComponent(jToolBar, javax.swing.GroupLayout.PREFERRED_SIZE,
                60, javax.swing.GroupLayout.PREFERRED_SIZE).addPreferredGap(
                javax.swing.LayoutStyle.ComponentPlacement.RELATED).addComponent(
                tabbedPane, javax.swing.GroupLayout.DEFAULT_SIZE, 350,
                Short.MAX_VALUE).addContainerGap()));

        // status bar initialization - message timeout, idle icon and busy animation, etc
        ResourceMap resourceMap = getResourceMap();
        int messageTimeout = resourceMap.getInteger("StatusBar.messageTimeout");
        messageTimer = new Timer(messageTimeout, new ActionListener() {

            public void actionPerformed(ActionEvent e) {
                statusMessageLabel.setText("");
            }
        });
        messageTimer.setRepeats(false);
        int busyAnimationRate = resourceMap.getInteger("StatusBar.busyAnimationRate");
        for (int i = 0; i < busyIcons.length; i++) {
            busyIcons[i] = resourceMap.getIcon("StatusBar.busyIcons[" + i + "]");
        }
        busyIconTimer = new Timer(busyAnimationRate, new ActionListener() {

            public void actionPerformed(ActionEvent e) {
                busyIconIndex = (busyIconIndex + 1) % busyIcons.length;
                statusAnimationLabel.setIcon(busyIcons[busyIconIndex]);
            }
        });
        idleIcon = resourceMap.getIcon("StatusBar.idleIcon");
        statusAnimationLabel.setIcon(idleIcon);
        progressBar.setVisible(false);

        // connecting action tasks to status bar via TaskMonitor
        TaskMonitor taskMonitor = new TaskMonitor(getApplication().getContext());
        taskMonitor.addPropertyChangeListener(new java.beans.PropertyChangeListener() {

            public void propertyChange(java.beans.PropertyChangeEvent evt) {
                String propertyName = evt.getPropertyName();
                if ("started".equals(propertyName)) {
                    if (!busyIconTimer.isRunning()) {
                        statusAnimationLabel.setIcon(busyIcons[0]);
                        busyIconIndex = 0;
                        busyIconTimer.start();
                    }
                    progressBar.setVisible(true);
                    progressBar.setIndeterminate(true);
                } else if ("done".equals(propertyName)) {
                    busyIconTimer.stop();
                    statusAnimationLabel.setIcon(idleIcon);
                    progressBar.setVisible(false);
                    progressBar.setValue(0);
                } else if ("message".equals(propertyName)) {
                    String text = (String) (evt.getNewValue());
                    statusMessageLabel.setText((text == null) ? "" : text);
                    messageTimer.restart();
                } else if ("progress".equals(propertyName)) {
                    int value = (Integer) (evt.getNewValue());
                    progressBar.setVisible(true);
                    progressBar.setIndeterminate(false);
                    progressBar.setValue(value);
                }
            }
        });
        PIPApp.getApplication().getMainFrame().addMouseListener(new MouseListener() {

            @Override
            public void mouseClicked(MouseEvent arg0) {
                // TODO Auto-generated method stub
            }

            @Override
            public void mouseEntered(MouseEvent arg0) {
                // TODO Auto-generated method stub
            }

            @Override
            public void mouseExited(MouseEvent arg0) {
                // TODO Auto-generated method stub
            }

            @Override
            public void mousePressed(MouseEvent arg0) {
                // TODO Auto-generated method stub
            }

            @Override
            public void mouseReleased(MouseEvent arg0) {
                PIPApp.getApplication().getMainFrame().revalidate();
            }
        });
    }

    @Action
    public void showAboutBox() {
        if (aboutBox == null) {
            JFrame mainFrame = PIPApp.getApplication().getMainFrame();
            aboutBox = new PIPAboutBox(mainFrame);
            aboutBox.setLocationRelativeTo(mainFrame);
        }
        PIPApp.getApplication().show(aboutBox);
    }

    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        mainPanel = new javax.swing.JPanel();
        jToolBar = new javax.swing.JToolBar();
        openToolButton = new javax.swing.JButton();
        grayScaleToolButton = new javax.swing.JToggleButton();
        histogramToolButton = new javax.swing.JToggleButton();
        menuBar = new javax.swing.JMenuBar();
        javax.swing.JMenu fileMenu = new javax.swing.JMenu();
        openMenuItem = new javax.swing.JMenuItem();
        javax.swing.JMenuItem exitMenuItem = new javax.swing.JMenuItem();
        imageMenu = new javax.swing.JMenu();
        grayScaleMenuItem = new javax.swing.JMenuItem();
        histogramMenuItem = new javax.swing.JMenuItem();
        javax.swing.JMenu helpMenu = new javax.swing.JMenu();
        javax.swing.JMenuItem aboutMenuItem = new javax.swing.JMenuItem();
        statusPanel = new javax.swing.JPanel();
        javax.swing.JSeparator statusPanelSeparator = new javax.swing.JSeparator();
        statusMessageLabel = new javax.swing.JLabel();
        statusAnimationLabel = new javax.swing.JLabel();
        progressBar = new javax.swing.JProgressBar();
        fileChooser = new javax.swing.JFileChooser();

        mainPanel.setName("mainPanel"); // NOI18N

        jToolBar.setFocusable(false);
        jToolBar.setName("jToolBar"); // NOI18N

        javax.swing.ActionMap actionMap = org.jdesktop.application.Application.getInstance(pip.PIPApp.class).getContext().getActionMap(PIPView.class, this);
        openToolButton.setAction(actionMap.get("openImage")); // NOI18N
        org.jdesktop.application.ResourceMap resourceMap = org.jdesktop.application.Application.getInstance(pip.PIPApp.class).getContext().getResourceMap(PIPView.class);
        openToolButton.setIcon(resourceMap.getIcon("openToolButton.icon")); // NOI18N
        openToolButton.setFocusable(false);
        openToolButton.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
        openToolButton.setLabel(resourceMap.getString("openToolButton.label")); // NOI18N
        openToolButton.setName("openToolButton"); // NOI18N
        openToolButton.setVerticalTextPosition(javax.swing.SwingConstants.BOTTOM);
        jToolBar.add(openToolButton);

        grayScaleToolButton.setAction(actionMap.get("grayScale")); // NOI18N
        grayScaleToolButton.setIcon(resourceMap.getIcon("grayScaleToolButton.icon")); // NOI18N
        grayScaleToolButton.setText(resourceMap.getString("grayScaleToolButton.text")); // NOI18N
        grayScaleToolButton.setToolTipText(resourceMap.getString("grayScaleToolButton.toolTipText")); // NOI18N
        grayScaleToolButton.setEnabled(false);
        grayScaleToolButton.setFocusable(false);
        grayScaleToolButton.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
        grayScaleToolButton.setName("grayScaleToolButton"); // NOI18N
        grayScaleToolButton.setVerticalTextPosition(javax.swing.SwingConstants.BOTTOM);
        jToolBar.add(grayScaleToolButton);

        histogramToolButton.setAction(actionMap.get("histogram")); // NOI18N
        histogramToolButton.setIcon(resourceMap.getIcon("histogramToolButton.icon")); // NOI18N
        histogramToolButton.setEnabled(false);
        histogramToolButton.setFocusable(false);
        histogramToolButton.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
        histogramToolButton.setLabel(resourceMap.getString("histogramToolButton.label")); // NOI18N
        histogramToolButton.setName("histogramToolButton"); // NOI18N
        histogramToolButton.setVerticalTextPosition(javax.swing.SwingConstants.BOTTOM);
        jToolBar.add(histogramToolButton);

        javax.swing.GroupLayout mainPanelLayout = new javax.swing.GroupLayout(mainPanel);
        mainPanel.setLayout(mainPanelLayout);
        mainPanelLayout.setHorizontalGroup(
            mainPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(jToolBar, javax.swing.GroupLayout.DEFAULT_SIZE, 753, Short.MAX_VALUE)
        );
        mainPanelLayout.setVerticalGroup(
            mainPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(mainPanelLayout.createSequentialGroup()
                .addComponent(jToolBar, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(366, Short.MAX_VALUE))
        );

        menuBar.setName("menuBar"); // NOI18N

        fileMenu.setText(resourceMap.getString("fileMenu.text")); // NOI18N
        fileMenu.setName("fileMenu"); // NOI18N

        openMenuItem.setAction(actionMap.get("openImage")); // NOI18N
        openMenuItem.setIcon(resourceMap.getIcon("openMenuItem.icon")); // NOI18N
        openMenuItem.setText(resourceMap.getString("openMenuItem.text")); // NOI18N
        openMenuItem.setName("openMenuItem"); // NOI18N
        fileMenu.add(openMenuItem);

        exitMenuItem.setAction(actionMap.get("quit")); // NOI18N
        exitMenuItem.setName("exitMenuItem"); // NOI18N
        fileMenu.add(exitMenuItem);

        menuBar.add(fileMenu);

        imageMenu.setText(resourceMap.getString("imageMenu.text")); // NOI18N
        imageMenu.setName("imageMenu"); // NOI18N

        grayScaleMenuItem.setAction(actionMap.get("grayScale")); // NOI18N
        grayScaleMenuItem.setIcon(resourceMap.getIcon("grayScaleMenuItem.icon")); // NOI18N
        grayScaleMenuItem.setText(resourceMap.getString("grayScaleMenuItem.text")); // NOI18N
        grayScaleMenuItem.setEnabled(false);
        grayScaleMenuItem.setName("grayScaleMenuItem"); // NOI18N
        imageMenu.add(grayScaleMenuItem);

        histogramMenuItem.setAction(actionMap.get("histogram")); // NOI18N
        histogramMenuItem.setText(resourceMap.getString("histogramMenuItem.text")); // NOI18N
        histogramMenuItem.setEnabled(false);
        histogramMenuItem.setName("histogramMenuItem"); // NOI18N
        imageMenu.add(histogramMenuItem);

        menuBar.add(imageMenu);

        helpMenu.setText(resourceMap.getString("helpMenu.text")); // NOI18N
        helpMenu.setName("helpMenu"); // NOI18N

        aboutMenuItem.setAction(actionMap.get("showAboutBox")); // NOI18N
        aboutMenuItem.setName("aboutMenuItem"); // NOI18N
        helpMenu.add(aboutMenuItem);

        menuBar.add(helpMenu);

        statusPanel.setName("statusPanel"); // NOI18N

        statusPanelSeparator.setName("statusPanelSeparator"); // NOI18N

        statusMessageLabel.setName("statusMessageLabel"); // NOI18N

        statusAnimationLabel.setHorizontalAlignment(javax.swing.SwingConstants.LEFT);
        statusAnimationLabel.setName("statusAnimationLabel"); // NOI18N

        progressBar.setName("progressBar"); // NOI18N

        javax.swing.GroupLayout statusPanelLayout = new javax.swing.GroupLayout(statusPanel);
        statusPanel.setLayout(statusPanelLayout);
        statusPanelLayout.setHorizontalGroup(
            statusPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(statusPanelSeparator, javax.swing.GroupLayout.DEFAULT_SIZE, 753, Short.MAX_VALUE)
            .addGroup(statusPanelLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(statusMessageLabel)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 583, Short.MAX_VALUE)
                .addComponent(progressBar, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(statusAnimationLabel)
                .addContainerGap())
        );
        statusPanelLayout.setVerticalGroup(
            statusPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(statusPanelLayout.createSequentialGroup()
                .addComponent(statusPanelSeparator, javax.swing.GroupLayout.PREFERRED_SIZE, 2, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGroup(statusPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(statusMessageLabel)
                    .addComponent(statusAnimationLabel)
                    .addComponent(progressBar, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(3, 3, 3))
        );

        fileChooser.setName("fileChooser"); // NOI18N

        setComponent(mainPanel);
        setMenuBar(menuBar);
        setStatusBar(statusPanel);
    }// </editor-fold>//GEN-END:initComponents

    @Action
    public Task openImage() {
        return new OpenImageTask(getApplication());
    }

    /**
     * @return the imageProcessor
     */
    public ImageProcessor getImageProcessor() {
        return imageProcessor;
    }

    /**
     * @return the fileName
     */
    public String getFileName() {
        return fileName;
    }

    /**
     * @param fileName the fileName to set
     */
    public void setFileName(String fileName) {
        this.fileName = fileName;
    }

    private class OpenImageTask extends org.jdesktop.application.Task<Object, Void> {

        private int fileChooserVal;

        OpenImageTask(org.jdesktop.application.Application app) {
            super(app);
            fileChooserVal = fileChooser.showOpenDialog(null);
            if (fileChooserVal == JFileChooser.APPROVE_OPTION) {
                File file = fileChooser.getSelectedFile();
                fileName = file.getAbsolutePath();
            }
        }

        @Override
        protected Object doInBackground() {
            if (fileChooserVal == JFileChooser.APPROVE_OPTION) {
                // Load image
                imageProcessor = new ImageProcessor(fileName);

                // Show UI
                if (originPanel != null) {
                    tabbedPane.removeAll();
                    grayScaleToolButton.setSelected(false);
                    histogramToolButton.setSelected(false);
                }
                BufferedImage image = imageProcessor.getBufferedImage();
                originPanel = new ImagePanel(image);
                tabbedPane.addTab("Origin image", originPanel);
                mainPanel.add(tabbedPane);
                mainPanel.repaint();
                mainPanel.revalidate();
            }
            return null;
        }

        @Override
        protected void succeeded(Object result) {
            if (fileChooserVal == JFileChooser.APPROVE_OPTION) {
                grayScaleToolButton.setEnabled(true);
                grayScaleMenuItem.setEnabled(true);
                histogramToolButton.setEnabled(true);
                histogramMenuItem.setEnabled(true);
            }
        }
    }

    @Action
    public Task grayScale() {
        return new GrayScaleTask(getApplication());
    }

    private class GrayScaleTask extends org.jdesktop.application.Task<Object, Void> {

        GrayScaleTask(org.jdesktop.application.Application app) {
            super(app);
        }

        @Override
        protected Object doInBackground() {
            if (tabbedPane.indexOfComponent(grayScalePanel) > 0) {
                // tabbedPane contain gray scale
                tabbedPane.remove(grayScalePanel);
            } else {
                // add to tabbedPane
                BufferedImage grayScaleImage = imageProcessor.getGrayScaleImage(null);
                grayScalePanel = new ImagePanel(grayScaleImage);
                tabbedPane.addTab("Gray scale image", grayScalePanel);
                tabbedPane.setSelectedComponent(grayScalePanel);
            }
            return null;
        }

        @Override
        protected void succeeded(Object result) {
        }
    }

    @Action
    public Task histogram() {
        return new HistogramTask(getApplication());
    }

    private class HistogramTask extends org.jdesktop.application.Task<Object, Void> {

        HistogramTask(org.jdesktop.application.Application app) {
            super(app);
        }

        @Override
        protected Object doInBackground() {
            if (tabbedPane.indexOfComponent(histogramPanel) > 0) {
                // tabbedPane contain histogram
                tabbedPane.remove(histogramPanel);
            } else {
                // add to tabbedPane
                histogramPanel = new HistogramPanel(imageProcessor);
                tabbedPane.addTab("Histogram", histogramPanel);
                tabbedPane.setSelectedComponent(histogramPanel);
            }
            return null;
        }

        @Override
        protected void succeeded(Object result) {
        }
    }

    public static class ThresholdListener implements ActionListener {

        public void actionPerformed(ActionEvent e) {
            if (tabbedPane.indexOfTabComponent(thresholdPanel) > 0) {
                // tabbedPane contain thresholdPanel
                tabbedPane.remove(thresholdPanel);
            } else {
                // add to tabbedPane
                int threshold = HistogramPanel.getThresholdValue();
                thresholdPanel = new ImagePanel(
                        imageProcessor.getThresholdImage(threshold));
                tabbedPane.addTab("Binary image", thresholdPanel);
                tabbedPane.setSelectedComponent(thresholdPanel);
            }
        }
    }
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JFileChooser fileChooser;
    private javax.swing.JMenuItem grayScaleMenuItem;
    private javax.swing.JToggleButton grayScaleToolButton;
    private javax.swing.JMenuItem histogramMenuItem;
    private javax.swing.JToggleButton histogramToolButton;
    private javax.swing.JMenu imageMenu;
    private javax.swing.JToolBar jToolBar;
    private javax.swing.JPanel mainPanel;
    private javax.swing.JMenuBar menuBar;
    private javax.swing.JMenuItem openMenuItem;
    private javax.swing.JButton openToolButton;
    private javax.swing.JProgressBar progressBar;
    private javax.swing.JLabel statusAnimationLabel;
    private javax.swing.JLabel statusMessageLabel;
    private javax.swing.JPanel statusPanel;
    // End of variables declaration//GEN-END:variables
    private final Timer messageTimer;
    private final Timer busyIconTimer;
    private final Icon idleIcon;
    private final Icon[] busyIcons = new Icon[15];
    private int busyIconIndex = 0;
    private JDialog aboutBox;
    private static JTabbedPane tabbedPane;
    private ImagePanel originPanel;
    private ImagePanel grayScalePanel;
    private HistogramPanel histogramPanel;
    private static JPanel thresholdPanel;
    private String fileName;
    private static ImageProcessor imageProcessor;
}
