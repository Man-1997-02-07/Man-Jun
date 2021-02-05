<template>
  <section class="netDisk-page">
    <div
      v-show="showBlockPic"
      class="block-desktop">
      <img src="@/assets/images/icons/block_storage.png">
      <p>{{ $t('resource.noData') }}</p>
    </div>
    <vx-box
      v-show="!showBlockPic"
      :header-line="true"
      style="height:100%;">
      <template
        slot="header">
        <span class="title">{{ $t('disk.diskFile') }}</span>
        <div class="f-right">
          <el-input
            v-if="isFromVolume"
            id="volume_networkBox_filterName"
            v-model="filtersVal"
            :placeholder="$t('disk.searchHolder')"
            style="width:280px;vertical-align: middle;margin-right:6px;"
            class="selectType"
            @focus="isFocuFileName = true"
            @blur="isFocuFileName = false">
            <i
              v-if="filtersVal !== ''"
              id="volume_networkBox_clearName"
              slot="suffix"
              class="el-input__icon el-icon-error"
              style="position:absolute;top:-5px;left:-20px;"
              @click="clearFilter" />
            <el-button
              id="volume_networkBox_filterBtn"
              slot="append"
              @click="searchBucketObject">
              <i class="el-icon-search" />{{ $t('disk.seach') }}
            </el-button>
          </el-input>
          <!-- :disabled="isRename || (!isadmin && !isShow)" -->
          <el-button
            v-if="isFromVolume"
            id="volume_networkBox_creat"
            :disabled="isRename || (!isadmin && !isShow)"
            @click="handleNetworkBoxCreate">
            <i class="el-icon-folder-add" />
            {{ $t('disk.addFolder') }}
          </el-button>
          <el-dropdown v-if="isShow && isFromVolume && canUpload && cur_file_auth!=='READ'">
            <el-button
              v-if="isWin"
              id="volume_networkBox_uploadOptFile"
              size="small"
              style="padding:6px 10px;">
              <i class="el-icon-upload2" />
              {{ $t('disk.uploadFiles') }}
            </el-button>
            <el-dropdown-menu slot="dropdown">
              <el-dropdown-item>
                <!-- 上传文件 -->
                <el-upload
                  id="volume_networkBox_uploadOptFile2"
                  :action="object_upload_name"
                  :http-request="uploadFile"
                  :show-file-list="false">
                  <i class="el-icon-upload2" />
                  <span>{{ $t('disk.uploadFiles') }}</span>
                </el-upload>
              </el-dropdown-item>
              <el-dropdown-item>
                <input
                  id="volume_networkBox_uploadOptFolder"
                  type="file"
                  name="file"
                  directory
                  webkitdirectory
                  mozdirectory
                  style="position:absolute;opacity:0;left:0;width:100%;height:36px;cursor:pointer;"
                  @change="submintfiles">
                <i class="el-icon-upload2" />
                {{ $t('disk.uploadFolder') }}
              </el-dropdown-item>
            </el-dropdown-menu>
          </el-dropdown>
          <el-button
            v-if="isFromVolume && networkDataSelect.length > 0 && isShow"
            id="volume_networkBox_moveMultiple"
            @click="handleNetworkMoveMultiple(2)">
            <i
              class="sg-iconfont im-icon-yidong2"
              style="color: #3583E3; font-size: 12px;" />
            {{ $t('disk.remove') }}
          </el-button>
          <el-button
            v-if="isFromVolume && networkDataSelect.length > 0 && isShow"
            id="volume_networkBox_copyMultiple"
            @click="handleNetworkCopyMultiple(2)">
            <i class="el-icon-document-copy" />
            {{ $t('disk.copy') }}
          </el-button>
          <el-button
            v-if="networkDataSelect.length > 0&&isWin"
            id="volume_networkBox_download"
            @click="handleNetworkDowanload()">
            <i class="el-icon-download" />
            {{ $t('disk.download') }}
          </el-button>
          <!--删除-->
          <el-button
            v-if="isFromVolume && networkDataSelect.length > 0"
            id="volume_networkBox_DelMultiple"
            @click="handleNetworkBoxDelMultiple">
            <i class="el-icon-delete" />
            {{ $t('disk.del') }}
          </el-button>
          <el-button
            v-if="isFromVolume"
            id="volume_networkBox_refresh"
            :disabled="isShowUpload"
            @click="handleNetworkBoxRefresh">
            <i class="el-icon-refresh" />
            {{ $t('disk.refresh') }}
          </el-button>
          <!-- 上传列表 -->
          <el-button
            v-if="isFromVolume&&isWin"
            id="volume_networkBox_progress"
            @click="isShowUpload = !isShowUpload">
            <i class="el-icon-notebook-2" />
            {{ $t('disk.uploadList') }}
          </el-button>
          <!-- <el-button
            v-if="isFromVolume"
            id="volume_download_progress"
            @click="dialogDownloadListVisible = !dialogDownloadListVisible">
            <i class="el-icon-notebook-2" />
            下载列表
          </el-button> -->
        </div>
      </template>
      <div
        v-if="!isShowModel"
        class="networkBox_list--mode"
        style="z-index:2; height: 85%">
        <div style="margin-bottom:10px;">
          <span v-if="isShow">
            <span>
              <span
                id="volume_networkBox_list--goback"
                style="color:#3583E3; vertical-align: top;"
                @click="backBeforeBucket">{{ $t('disk.back') }}</span>
              <span> | </span>
            </span>
            <span
              id="volume_networkBox_list--all"
              style="color:#3583E3; vertical-align: top;"
              @click="getAllBuckets">{{ $t('disk.allFiles') }}</span>
            <div class="pathBox">
              <span
                v-for="(item,index) in cur_buckets"
                :key="index"
                class="goBucketPath">
                <span
                  id="volume_networkBox_bucketPath"
                  @click="goBucketPath(item,index)"> > {{ item }}</span>
              </span>
            </div>
          </span>
          <span v-else>
            <!-- <span
            id="volume_networkBox_goVolumes"
            style="color:#3583E3"
            @click="backBeforeVolume">返回存储卷</span> |  -->
            {{ $t('disk.allFiles') }}</span>
          <span
            v-show="uploadLoading"
            class="f-right"
            style="color:#3583E3;margin-top:0"><i class="el-icon-loading" />{{ $t('disk.dataCheck') }}</span>
          <span
            v-show="!uploadLoading&&language=='zh'"
            class="f-right"
            style="margin-top:0">已全部加载，共<span class="networkBox_list--num">{{ cur_fileLength }}</span>个</span>
          <span
            v-show="!uploadLoading&&language=='en'"
            class="f-right"
            style="margin-top:0">{{ cur_fileLength }} loading complete</span>
        </div>
        <div
          v-if="isBoxShaow"
          class="boxShaow">
          <span v-show="language=='zh'">已选中{{ fileSelect }}个文件/文件夹</span>
          <span v-show="language=='en'">{{ fileSelect }} files/folders selected</span>
        </div>
        <el-table
          id="volume_networkBox_list--mode"
          ref="netDiskTable"
          :data="files"
          height="100%"
          tooltip-effect="light"
          style="z-index:2"
          @selection-change="handleSelectionChange"
          @cell-click="checknetworkDiskFiels">
          <el-table-column
            type="selection"
            width="55" />
          <el-table-column
            prop="name"
            :label="$t('disk.fileName')"
            :show-overflow-tooltip="true">
            <template slot-scope="scope">
              <span>
                <div
                  v-if="isNewFiles && scope.row.newCreat"
                  class="volume_networkBox_list--createFile">
                  <el-input
                    id="volume_networkBox_list--filename"
                    v-model="fileName"
                    :placeholder="$t('disk.searchHolder')"
                    autofocus />
                  <span
                    id="volume_networkBox_newCreat"
                    @click.stop="confirmCreatNewFiles"><i class="el-icon-circle-check health" /></span>
                  <span
                    v-if="!isRename"
                    id="volume_networkBox_cancelNewCreat"
                    @click.stop="cancelCreatNewFiles"><i class="el-icon-error danger" /></span>
                  <span
                    v-if="isRename"
                    id="volume_networkBox_cancelRename"
                    @click.stop="cancelRenameFiles"><i class="el-icon-error danger" /></span>
                </div>
                <span v-else>
                  <img
                    class="netDisk-icon"
                    :src="getFileIcon(scope.row.type)">
                  {{ scope.row.name }}
                </span>
              </span>
            </template>
          </el-table-column>
          <el-table-column
            prop="size"
            align="center"
            width="100px"
            :label="$t('disk.size')" />
          <el-table-column
            v-if="!isShow && isFromVolume"
            prop="StorageQuota"
            align="center"
            :label="$t('disk.quota')"
            width="100px">
            <template slot-scope="scope">
              <div
                v-if="scope.row.isEditStorage"
                style="vertical-align: middle;height: 30px;"
                class="volume_networkBox_list--createFile">
                <el-input
                  v-model="newStorage"
                  :placeholder="$t('disk.searchHolder')"
                  class="input-with-select">
                  <el-select
                    slot="append"
                    v-model="storageUnit"
                    :placeholder="$t('cloud.select')">
                    <el-option
                      label="MB"
                      value="MB" />
                    <el-option
                      label="GB"
                      value="GB" />
                  </el-select>
                </el-input>
                <span
                  id="volume_networkBox_editStorage"
                  @click.stop="confirmEditStorage(scope.row)"><i class="el-icon-circle-check health" /></span>
                <span
                  id="volume_networkBox_cancelEditStorage"
                  @click.stop="cancelEditStorage(scope.row)"><i class="el-icon-error danger" /></span>
              </div>
              <span v-else>{{ getStorage(scope.row.StorageQuota) }} </span>
            </template>
          </el-table-column>
          <!-- 文件权限 -->
          <el-table-column
            prop="access"
            align="center"
            width="110"
            :label="$t('disk.curAuthority')">
            <template slot-scope="scope">
              {{ getAuthName(scope.row.access) }}
            </template>
          </el-table-column>
          <el-table-column
            prop="mtime"
            align="center"
            :label="$t('disk.modifiedTime')"
            width="150"
            :show-overflow-tooltip="true" />
          <el-table-column
            align="center"
            :label="$t('disk.opt')"
            width="110">
            <template slot-scope="scope">
              <template v-if="isNewFiles">
                <span>-</span>
              </template>
              <template v-else>
                <span
                  v-if="scope.row.type !== 'folder' && isFromVolume"
                  id="volume_networkBox_list--share"
                  class="oprateIcon"
                  @click.stop="handleNetworkShare(scope.row)">
                  <i class="icon_colorful sg-iconfont im-icon-fenxiang" />
                </span>
                <span
                  v-if="isWin"
                  id="volume_networkBox_list--download"
                  class="oprateIcon"
                  @click.stop="handleNetworkDowanloadSingle(scope.row)">
                  <i class="icon_colorful sg-iconfont im-icon-xiazai" />
                </span>
                <el-dropdown
                  v-if="isFromVolume"
                  id="volume_networkBox_list--more"
                  trigger="click"
                  class="volume_networkBox_list--more">
                  <span>
                    <i
                      style="font-size: 6px; color: #3583E3;"
                      class="icon_colorful sg-iconfont im-icon-gengduo" />
                  </span>
                  <el-dropdown-menu
                    slot="dropdown"
                    class="networkDisk-dropdown-menu">
                    <el-dropdown-item
                      id="volume_networkBox_list--move"
                      @click.native="handleNetworkBoxMove(scope.row,1)">
                      <i
                        class="sg-iconfont im-icon-yidong2"
                        style="font-size: 12px;" />
                      {{ $t('disk.moveTo') }}
                    </el-dropdown-item>
                    <el-dropdown-item
                      id="volume_networkBox_list--copy"
                      @click.native="handleNetworkBoxCopy(scope.row,1)">
                      <i class="el-icon-document-copy" />
                      {{ $t('disk.copyto') }}
                    </el-dropdown-item>
                    <el-dropdown-item
                      id="volume_networkBox_list--rename"
                      :disabled="!isAdd"
                      @click.native="handleNetworkBoxRename(scope.row)">
                      <i class="el-icon-edit" />
                      {{ $t('disk.rename') }}
                    </el-dropdown-item>
                    <el-dropdown-item
                      id="volume_networkBox_list--del"
                      @click.native="handleNetworkBoxDel(scope.row)">
                      <i class="el-icon-delete" />
                      {{ $t('disk.del') }}
                    </el-dropdown-item>
                    <el-dropdown-item
                      v-if="(cur_buckets.length === 0) && (scope.row.path?scope.row.path.split('/').length===3:true) && isadmin"
                      @click.native="distribute(scope.row)">
                      <i class="im-icon-allot" />
                      {{ $t('disk.assign') }}
                    </el-dropdown-item>
                    <el-dropdown-item
                      v-if="(cur_buckets.length === 0) && (scope.row.path?scope.row.path.split('/').length===3:true) && isadmin"
                      @click.native="editStorage(scope.row)">
                      <i class="im-icon-edit" />{{ $t('disk.editQuota') }}
                    </el-dropdown-item>
                  </el-dropdown-menu>
                </el-dropdown>
              </template>
            </template>
          </el-table-column>
        </el-table>
        <el-pagination
          v-if="isShow"
          id="volume_networkBox_page"
          layout="total, sizes, prev, pager, next, jumper"
          :page-sizes="[15, 30, 50, 80, 100]"
          :current-page.sync="pagenum"
          :page-size="pagesize"
          :total="total-notMatchCurFolderAccount"
          @size-change="handleSizeChange"
          @current-change="handleCurrentChange" />
      </div>
      <div
        id="dragable"
        @dragleave="handleDragLeave">
        <div class="dragable_upload--text">
          {{ $t('disk.uploadCurFolder') }}
        </div>
        <!-- <el-upload class="upload-demo" id="volume_networkBox_list--drag" drag :action="object_upload_name" :http-request="uploadFile" :show-file-list="false" v-if="isShow && isUpload">
          <div class="el-upload__text">上传文件到当前目录下</div>
        </el-upload> -->
      </div>
    </vx-box>
    <div
      v-if="isShowUpload"
      id="volume_networkBox_uploadOptList"
      class="netDisk-progress"
      @mouseover="isShowUpload = true"
      @mouseleave="isShowUpload = false">
      <el-table
        :data="[...upload_fileList].reverse()"
        tooltip-effect="light"
        height="90%">
        <el-table-column
          prop="name"
          :label="$t('disk.fileName')"
          :show-overflow-tooltip="true">
          <template slot-scope="scope">
            <img
              class="netDisk-icon"
              :src="getFileIcon(scope.row.type)">
            <i :class="['netDisk-icon', 'sg-iconfont', 'im-icon-'+getFileIcon(scope.row.type)]" />
            {{ scope.row.name }}
          </template>
        </el-table-column>
        <!-- <el-table-column prop="size" label="大小"></el-table-column> -->
        <el-table-column
          prop="mtime"
          :label="$t('disk.progress')"
          width="125px">
          <template slot-scope="scope">
            <span v-if="scope.row.isSuccess">
              <template v-if="scope.row.percentage < 100">
                <el-progress
                  type="circle"
                  :stroke-width="5"
                  :percentage="scope.row.percentage" />
                <span>
                  <el-tooltip :content="$t('disk.cancelUpload')">
                    <i
                      class="sg-iconfont im-icon-guaqi"
                      style="    position: absolute;  right: 45px;top: 15px;"
                      @click="stopUpload(scope.row)" /></el-tooltip>
                </span>
              </template>
              <template v-else>{{ $t('disk.uploadFinish') }}</template>
            </span>
            <span
              v-else
              style="color:#df1130;">
              {{ $t('disk.uploadFailed') }}
            </span>
          </template>
        </el-table-column>
      </el-table>
      <div class="upload-capacity">
        <div style="margin-bottom:5px;">
          <span>{{ $t('disk.used') }}{{ (upload.used/(1024*1024) > 1) ? (upload.used/(1024*1024)).toFixed(2) + 'G' : (upload.used/1024).toFixed(2) + 'M' }}</span>
          <span class="fr">{{ $t('disk.total') }}{{ (upload.total/(1024*1024) > 1) ? (upload.total/(1024*1024)).toFixed(2) + 'G' : (upload.total/1024).toFixed(2) + 'M' }}</span>
        </div>
        <el-progress
          :percentage="parseFloat((upload.percentage*100).toFixed(2))"
          :text-inside="true"
          :stroke-width="15"
          :show-text="false" />
      </div>
    </div>
    <!-- 复制对话框 -->
    <copyFile-Dialog
      ref="copyObject"
      :dialog-copy-visible.sync="dialogCopyVisible"
      :netDiskOpttVisible="netDiskOpttVisible"
      :all-buckets.sync="allBuckets"
      :awsS3Client="awsS3Client"
      :copy-file-obj="copyFileObj"
      :copy-type="copyType"
      :batch-select-obj="batchSelectObj"
      :cur_buckets="cur_buckets"
      :copy-dia-files="copyDiaFiles"
      :is-not-click="isNotClick"
      :curClusterUuid="curClusterUuid"
      @updateDiskInfo="getNetworkDiskInfo"
      @updateBucketObject="getBucketObject"
      @updateBackBefore="backBeforeBucket"
      @updateAllBuckets="getAllBuckets"
      @cancelNetDiskInfo="cancelNetDiskInfo"
      @bucketPathChange="bucketPathChange"
      @getNetDiskFilesOpt="getNetDiskFilesOpt" />
    <!-- 移动 -->
    <moveFile-Dialog
      ref="moveObject"
      :dialog-move-visible.sync="dialogMoveVisible"
      :netDiskOpttVisible="netDiskOpttVisible"
      :awsS3Client="awsS3Client"
      :batch-select-obj="batchSelectObj"
      :move-type="moveType"
      :move-file-obj="moveFileObj"
      :all-buckets.sync="allBuckets"
      :cur_buckets="cur_buckets"
      :move-dia-files="moveDiaFiles"
      :is-not-click="isNotClick"
      :curClusterUuid="curClusterUuid"
      @updateDiskInfo="getNetworkDiskInfo"
      @updateBucketObject="getBucketObject"
      @updateBackBefore="backBeforeBucket"
      @updateAllBuckets="getAllBuckets"
      @cancelNetDiskInfo="cancelNetDiskInfo"
      @bucketPathChange="bucketPathChange"
      @getNetDiskFilesOpt="getNetDiskFilesOpt" />
    <!-- 分享链接对话框 -->
    <shareFile-Dialog
      ref="shareObject"
      :dialog-share-visible.sync="dialogShareVisible"
      :awsS3Client="awsS3Client"
      :curClusterUuid="curClusterUuid"
      :share-files="shareFiles"
      :getFilePath="getFilePath"
      :cur_buckets="cur_buckets" />
    <!-- 分配用户 -->
    <distribute-user-dialog
      ref="distributeUser"
      :dialog-distribute-visible.sync="dialogDistributeVisible"
      :aclList="aclList"
      :tenant_uuid="tenant_uuid"
      :volume_uuid="volume_uuid"
      :curClusterUuid="curClusterUuid"
      @toggleDialogDistribute="toggleDialogDistribute"
      @confirmDistribute="confirmDistribute" />
    <!-- 下载 -->
    <download-dialog
      ref="downloadObject"
      :dialog-download-visible.sync="dialogDownloadVisible"
      @confirmDownload="confirmDownload" />
    <downloadList-dialog
      ref="downloadListObject"
      :get-file-icon="getFileIcon"
      :file-types="fileTypes"
      :fileIcon="fileIcon"
      :dialog-download-list-visible.sync="dialogDownloadListVisible"
      :download_file-list="download_fileList"
    />
    <el-dialog
      id="volume_networkBox_HttpsDiaCancel"
      :title="$t('resource.tip')"
      :visible.sync="dialogHttpsVisible"
      width="500px">
      <div class="Https-content">
        <div class="Https-notice">
          <img
            src="@/assets/images/netdisk/notice.png"
            class="tab-img">
          <span>{{ $t('disk.httpsTip1') }}</span>
        </div>
        <p class="https-desc">
          此服务器无法证明它是{{ address }}；您计算机的操作系统不信任其安全证书。 出现此问题的原因可能是配置有误或您的连接被拦截了。
        </p>
        <!-- <span @click="openHttps(https)">{{ $t('disk.toSettingHttps') }} {{ https }}</span> -->
        <a
          id="volume_networkBox_httpsSet"
          :href="https"
          target="_blank">{{ $t('disk.toSettingHttps') }} {{ https }}</a>
        <p class="https-set">
          {{ $t('disk.httpsTip2') }}
        </p>
      </div>
    </el-dialog>
    <el-dialog
      id="volume_networkBox_move_copyOpt"
      :title="netDiskFilesOptType === 1 ? `正在对${ optFiles }个项目进行复制` : `正在对${ optFiles }个项目进行移动`"
      :visible.sync="netDiskOpttVisible"
      width="25%"
      :show-close="true"
      @close="cancelOpt">
      <!-- :show-close="false" -->
      <div v-if="language=='zh'">
        共有{{ optSameFiles }}个同名文件
      </div>
      <div v-else>
        {{ optSameFiles }} files with the same name
      </div>
      <ul>
        <li
          id="volume_networkBox_move_copyOpt_cover"
          @click.stop="coverAllFilesOpt">
          {{ $t('disk.replaceTargetFile') }}
        </li>
        <li
          id="volume_networkBox_move_copyOpt_jump"
          @click.stop="jumpAllFilesOpt">
          {{ $t('disk.skipFiles') }}
        </li>
      </ul>
      <div slot="footer">
        <el-button
          id="volume_networkBox_move_copyOpt_cancel"
          class="close-button"
          @click="netDiskOpttVisible = false">
          {{ $t('resource.cancel') }}
        </el-button>
      </div>
    </el-dialog>
    <el-dialog
      id="volume_networkBox_uploadOpt"
      :title="`正在将${ uploadFileArray.length }个项目上传至网盘`"
      :visible.sync="uploadOutVisible"
      :close-on-click-modal="false"
      width="25%"
      :show-close="true"
      @close="cancelOpt">
      <div v-if="language=='zh'">
        共有{{ tabelSameFiles.length }}个同名文件
      </div>
      <div v-else>
        {{ tabelSameFiles.length }} files with the same name
      </div>
      <ul>
        <li
          id="volume_networkBox_uploadOpt_cover"
          @click.stop="handleCoverFiles">
          {{ $t('disk.replaceTargetFile') }}
        </li>
        <li
          id="volume_networkBox_uploadOpt_jump"
          @click.stop="handleJumpFiles">
          {{ $t('disk.skipFiles') }}
        </li>
        <li
          id="volume_networkBox_uploadOpt_timeStamp"
          @click.stop="handleTimeStampFiles">
          {{ $t('disk.keepAll') }}
        </li>
      </ul>
    </el-dialog>
  </section>
</template>
<script>
import { mapState, mapMutations } from 'vuex'
import moveFileDialog from './components/fileMoveDialog'
import shareFileDialog from './components/fileShareDialog'
import copyFileDialog from './components/fileCopyDialog'
import downloadDialog from './components/fileDownloadDialog'
import downloadListDialog from './components/downloadListDialog'
import distributeUserDialog from './components/distributeUserDialog'
import uploadFile from './upload.js'
import bucketObject from './bucketObject.js'
import deleteFile from './delete.js'
const { shell } = require('electron')
const os = require('os')
export default {
  components: {
    'shareFile-Dialog': shareFileDialog,
    'moveFile-Dialog': moveFileDialog,
    'copyFile-Dialog': copyFileDialog,
    'distribute-user-dialog': distributeUserDialog,
    'download-dialog': downloadDialog,
    'downloadList-dialog': downloadListDialog
  },
  mixins: [System, uploadFile, bucketObject, deleteFile],
  data () {
    return {
      awsS3Client: null, // awsS3对象
      // 所有的文件夹、文件信息
      isFocuFilePath: false,
      isFocuFileName: false,
      filtersVal: '',
      files: [],
      isShow: false,
      fileSelect: 0,
      checked: false,
      isShowModel: false,
      isBoxShaow: false,
      networkDataSelect: [],
      dialogUpLoadVisible: false,
      dialogShareVisible: false, // 控制分享文件弹窗
      dialogMoveVisible: false,
      dialogDownloadVisible: false, // 控制下载弹窗
      dialogDownloadListVisible: false, // 控制下载列表弹窗
      download_fileList: [
        { name: '不知名.png', size: 1541545, percentage: 50, isSuccess: true, isDownloading: true, type: 'png' }
      ], // 下载列表
      curFile: {}, // 当前文件
      selectedFileObj: null,
      moveFileObj: null,
      copyFileObj: null,
      shareFiles: {},
      cur_fileLength: 0, // 当前页面下的文件（夹）数量
      cur_buckets: [],
      options: {},
      fileName: this.$t('disk.addFolder'),
      isNewFiles: false,
      cur_bucket: '',
      dialogCopyVisible: false,
      updateWsTimer: null,
      fileTypes: {
        folder: ['folder'],
        pdf: ['pdf'],
        txt: ['txt'],
        rar: ['rar', 'zip', '7z', 'iso', 'tar'],
        svg: ['svg'],
        img: ['bmp', 'jpg', 'jpeg', 'png', 'tif', 'jfif', 'gif', 'gif', 'pcx', 'tga', 'exif', 'fpx', 'svg', 'psd', 'cdr', 'pcd', 'dxf', 'ufo', 'eps', 'ai', 'raw', 'WMF', 'webp', 'ico'],
        xml: ['html', 'xml'],
        css: ['css', 'scss', 'less', 'sass'],
        doc: ['doc', 'docx'],
        ppt: ['ppt', 'pptx'],
        xlsx: ['xls', 'xlsx'],
        viso: ['vsd'],
        js: ['js']
      },
      fileIcon: {
        folder: require('../../assets/images/netdisk/wenjianjia.png'),
        pdf: require('../../assets/images/netdisk/PDF.png'),
        txt: require('../../assets/images/netdisk/txt1.png'),
        rar: require('../../assets/images/netdisk/rar.png'),
        svg: require('../../assets/images/netdisk/svg-.png'),
        img: require('../../assets/images/netdisk/picture.png'),
        css: require('../../assets/images/netdisk/css.png'),
        xml: require('../../assets/images/netdisk/html.png'),
        doc: require('../../assets/images/netdisk/doc.png'),
        ppt: require('../../assets/images/netdisk/PPT.png'),
        xlsx: require('../../assets/images/netdisk/xls.png'),
        viso: require('../../assets/images/netdisk/file-vsdx.png'),
        js: require('../../assets/images/netdisk/js.png'),
        weizhi: require('../../assets/images/netdisk/wenjianweizhi.png')
      },
      isAdd: true,
      moveType: 1,
      copyType: 1,
      downloadProgress: '',
      batchSelectObj: [],
      allBuckets: [],
      dialogProgressVisible: false,
      upload_fileList: [],
      isShowUpload: false,
      s3UploadRequest: null,
      cur_progress: true,
      upload: {
        percentage: 0,
        used: 0,
        total: 0
      },
      isRename: false,
      isSelected: false,
      curFolders: [],
      object_upload_name: '#',
      isUpload: false,
      pagenum: 1,
      pagesize: 15,
      total: 0,
      dragName: '',
      folderFiles: [],
      uploadLoading: false,
      dragFiles: [],
      uploadingTimer: null,
      dialogHttpsVisible: false,
      https: '',
      address: '',
      moveDiaFiles: [],
      copyDiaFiles: [],
      isUploadFile: false,
      uploadOutVisible: false, // 上传同名文件夹操作弹框
      netDiskFilesOptType: 1, // 1为复制操作，2为移动操作
      netDiskOpttVisible: false,
      optFiles: 0,
      optSameFiles: 0,
      whereFrom: '',
      dialogDistributeVisible: false,
      aclList: [],
      S3ObjKey: '',
      reqList: [],
      notMatchCurFolderAccount: 0, // 与当前目录不匹配的文件数量
      curClusterUuid: '',
      tenant_uuid: '',
      curAclBucket: {},
      volume_uuid: '',
      volume_name: '',
      canUpload: true, // 记录该桶对于本用户是否可写
      uploadErr: false, // 上传时是否报错
      storageUnit: 'MB',
      newStorage: 0,
      cur_netDisk_host: '',
      showBlockPic: true,
      curBucketObjList: [], // 当前桶对象列表
      cur_file_auth: '', // 当前文件类型 只读/读写
      isWin: true
    }
  },
  watch: {
    cur_progress (val) {
      const self = this
      if (val) {
        !self.uploadErr && (self.uploadLoading = true)
        setTimeout(function () {
          self.getBucketObject()
        }, 1000)
      }
    },
    cur_guide_item (val) {
      if (val == 'block_res') {
        this.showBlockPic = true
      } else {
        this.getInit()
      }
    }
  },
  mounted () {
    if (this.cur_guide_item == 'block_res') {
      this.showBlockPic = true
    } else {
      this.getInit()
    }
    if (!os.platform().includes('win')) {
      // linux下是全屏状态，不能上传下载
      this.isWin = false
    }
  },
  destroyed () {
    this.updateWsTimer && clearInterval(this.updateWsTimer)
    this.updateWsTimer = null
    this.uploadingTimer && clearInterval(this.uploadingTimer)
    this.uploadingTimer = null
    this.upload_fileList = this.upload_fileList.filter(item => (item.isSuccess && item.lastloaded < item.size))
    this.setUploadFileList({ key: this.S3ObjKey, upload_fileList: this.upload_fileList })
  },
  methods: {
    ...mapMutations('Netdisk', ['createS3Obeject', 'setUploadFileList']),
    getInit () {
      if (this.cur_guide_item && this.cur_guide_item.SharePoint) {
        this.showBlockPic = false
        this.files = []
        this.awsS3Client = null // 置空列表数据
        this.cur_buckets = []
        this.isShow = false
        this.getVolumeUnMount(this.cur_guide_item)
        this.volume_uuid = this.cur_guide_item.volume_uuid
        this.volume_name = this.cur_guide_item.volume_name
        this.curClusterUuid = this.cluster_uuid
        this.tenant_uuid = this.cur_guide_item.tenant ? this.cur_guide_item.tenant : this.cur_tenant
        // this.$nextTick(() => {
        //   this.getAllBuckets()
        // })
      } else {
        this.showBlockPic = true
        // this.$message({
        //   message: this.$t('resource.noData'),
        //   type: 'success',
        //   duration: 3000
        // })
      }
    },
    // 获取S3存储卷信息
    getVolumeUnMount (row) {
      const self = this
      self.setState({
        attr: 'innerLoading',
        val: true
      })
      const params = {
        tenant: row.tenant ? row.tenant : this.cur_tenant,
        volume_uuid: row.volume_uuid,
        page_num: 0,
        page_size: 0,
        cluster_uuid: row.cluster_uuid
      }
      self.cur_netDisk_host = ''
      this.$Api.netdisk.volumeMount(params).then((response) => {
        self.setState({
          attr: 'innerLoading',
          val: false
        })
        if (response.scode == 0) {
          const data = response.data
          if (data.mount_hosts.length > 0) { // 挂载列表
            self.cur_netDisk_host = data.mount_hosts[0].manager_net.cidr_addrs[0].split('/')[0]
            // self.cur_netDisk_host = self.networkDsikType === 'manager' ? data.mount_hosts[0].manager_net.cidr_addrs[0].split('/')[0] : data.mount_hosts[0].storage_net.cidr_addrs[0].split('/')[0] // 这里的ip拿的是数据ip
          }
          self.confirmSkyDrive(row)
        } else {
          self.$message({
            message: response.message_cn,
            type: 'error',
            duration: 3000
          })
        }
      })
    },
    // 确认进入网盘
    confirmSkyDrive (row) {
      const self = this
      let options = {}
      if (row.SharePoint) {
        options = {
          endPoint: self.cur_netDisk_host,
          port: parseInt(row.SharePoint.s3.split('#')[0]),
          useSSL: row.attr.EnableSSL === 'on' ? 'https' : 'http',
          accessKey: row.SharePoint.s3.split('#')[1],
          secretKey: row.SharePoint.s3.split('#')[2]
        }
        sessionStorage.setItem('netDisk', JSON.stringify(options))
        self.setState({
          attr: 'netDisk',
          val: JSON.stringify(options)
        })
        this.initData()
        this.s3OptionsInit()
        this.getNetworkDiskInfo()
      } else {
      }
    },
    initData () {
      const self = this
      self.S3ObjKey = (typeof self.netDisk) === 'string' ? JSON.parse(self.netDisk).accessKey : self.netDisk.accessKey// 记录key
      self.createS3Obeject({ key: self.S3ObjKey }) // vuex初始化对象
      self.upload_fileList = JSON.parse(JSON.stringify(self.S3Obeject[self.S3ObjKey].upload_fileList))
      self.upload_fileList.length && (self.isShowUpload = true)
    },
    // 统一配置s3对象
    s3OptionsInit () {
      const self = this
      let netDiskInfo = null
      if ((typeof self.netDisk) === 'string') {
        netDiskInfo = JSON.parse(self.netDisk)
      } else {
        netDiskInfo = self.netDisk
      }
      const credentials = {
        accessKeyId: netDiskInfo.accessKey,
        secretAccessKey: netDiskInfo.secretKey,
        region: 'us-east-1',
        s3ForcePathStyle: true
      }
      // 秘钥形式的登录上传
      if (!netDiskInfo.endPoint) {
        self.$message({
          message: '网盘状态不健康，暂时无法获取数据',
          type: 'warning',
          duration: 1500
        })
        return false
      }
      AWS.config.update(credentials)
      var ep = new AWS.Endpoint(`${netDiskInfo.useSSL}://${netDiskInfo.endPoint}:${parseInt(netDiskInfo.port)}`)
      self.awsS3Client = new AWS.S3({ endpoint: ep })
    },
    // 搜索文件夹下的文件
    searchBucketObject () {
      if (this.filtersVal !== '') {
        const params = {
          FilterName: encodeURI(this.filtersVal),
          FilterPath: ''
        }
        this.getNetworkDiskInfo(params)
      } else {
        this.$message({
          message: this.$t('disk.emptyfilename'),
          type: 'warning',
          duration: 3000
        })
      }
    },
    distribute (row) {
      const self = this
      self.curAclBucket = row
      self.$Api.system.updateWsToken({ cluster_uuid: self.cluster_uuid || self.curClusterUuid })
      self.setState({
        attr: 'onLoading',
        val: true
      })
      self.awsS3Client.getBucketAcl({ Bucket: row.name }, function (err, data) {
        // debugger
        self.setState({
          attr: 'onLoading',
          val: false
        })
        if (err) {
          self.$message({
            message: err,
            type: 'error',
            duration: 3000
          })
        } else {
          self.aclList = []
          data.Grants.forEach(item => {
            self.aclList.push({ uuid: item.Grantee.ID, name: item.Grantee.DisplayName, Permission: item.Permission })
          })
          self.toggleDialogDistribute(true)
        }
      })
    },
    getStorage (storage) {
      const unit = ['B', 'KB', 'MB', 'GB', 'TB']
      let num = 0
      while (storage > 1024) {
        num++
        storage /= 1024
      }
      return storage > 0 ? (storage.toFixed(2) + unit[num]) : '-'
    },
    getAuthName (auth) {
      if (!auth) {
        return this.language == 'zh' ? '未知' : 'Unknown'
      } else {
        if (auth == 'READ') {
          return this.language == 'zh' ? '只读' : 'Readonly'
        } else {
          return this.language == 'zh' ? '读写' : 'Read and write'
        }
      }
    },
    confirmEditStorage (row) {
      const self = this
      var reg = /^[0-9]+(\.\d+)?$/
      if (self.newStorage < 0 || !reg.test(self.newStorage)) {
        self.$message({
          message: self.$t('disk.greaterThanZore'),
          type: 'error',
          duration: 3000
        })
        return
      }
      let StorageQuota = self.newStorage
      if (self.storageUnit === 'MB') {
        StorageQuota *= (1024 * 1024)
      } else {
        StorageQuota *= (1024 * 1024 * 1024)
      }
      if (StorageQuota > self.upload.total * 1024) {
        self.$message({
          message: self.$t('disk.quotaLimit'),
          type: 'error',
          duration: 3000
        })
        return
      }
      self.$Api.system.updateWsToken({ cluster_uuid: self.cluster_uuid || self.curClusterUuid })
      self.awsS3Client.putBucketQuota({ Bucket: row.name, Quota: { StorageQuota: StorageQuota.toString() } }, function (err, data) {
        if (err) {
          self.$message({
            message: err,
            type: 'error',
            duration: 3000
          })
        } else {
          row.StorageQuota = StorageQuota
          self.$message({
            message: self.$t('disk.quotaEdit'),
            type: 'success',
            duration: 3000
          })
          self.$set(row, 'isEditStorage', false)
        }
      })
    },
    cancelEditStorage (row) {
      this.$set(row, 'isEditStorage', false)
    },
    editStorage (row) {
      this.$set(row, 'isEditStorage', true)
      this.newStorage = row.StorageQuota / (1024 * 1024)
      if (this.newStorage > 1024) {
        this.newStorage /= 1024
        this.storageUnit = 'GB'
      } else {
        this.storageUnit = 'MB'
      }
      !this.newStorage && (this.newStorage = 0)
    },
    // 打开私密连接
    // openHttps (url) {
    //   shell.openExternal(url)
    // },
    toggleDialogDistribute (isShow) {
      this.dialogDistributeVisible = isShow
    },
    confirmDistribute (list, ownerNames) {
      const self = this
      const params = {
        Bucket: self.curAclBucket.name,
        AccessControlPolicy: { Grants: [] }
      }
      list.forEach(item => {
        params.AccessControlPolicy.Grants.push(
          {
            Grantee: {
              DisplayName: item.name,
              ID: item.uuid,
              Type: 'CanonicalUser'
            },
            Permission: item.Permission
          }
        )
      })
      self.awsS3Client.putBucketAcl(params, function (err, data) {
        // debugger
        self.setState({
          attr: 'onLoading',
          val: false
        })
        self.toggleDialogDistribute(false)
        if (err) {
          self.$message({
            message: err,
            type: 'error',
            duration: 3000
          })
        } else {
          self.$message({
            message: self.$t('disk.assignUserSuccess'),
            type: 'success',
            duration: 3000
          })
        }
      })
    },
    // 清除文件路径搜索
    clearFilter () {
      this.filtersVal = ''
      if (this.isShow) {
        this.getBucketObject()
      } else {
        this.getAllBuckets()
      }
      this.isNewFiles = false
      this.isRename = false
      this.isAdd = true
      this.fileName = this.$t('disk.addFolder')
    },
    async refreshStorageUseData () {
      const self = this
      const params = {}
      self.$Api.system.updateWsToken({ cluster_uuid: self.cluster_uuid || self.curClusterUuid })
      self.s3UploadRequest = self.awsS3Client.listBuckets(params, function (err, data) {
        if (err) {
          let netDiskInfo = null
          if ((typeof self.netDisk) === 'string') {
            netDiskInfo = JSON.parse(self.netDisk)
          } else {
            netDiskInfo = self.netDisk
          }
          if (netDiskInfo.useSSL === 'https') {
            self.dialogHttpsVisible = true
            self.address = netDiskInfo.endPoint
            self.https = `${netDiskInfo.useSSL}://${netDiskInfo.endPoint}:${parseInt(netDiskInfo.port)}`
          } else {
            self.$message({
              message: err,
              type: 'error',
              duration: 1500
            })
          }
        }
      })
      await self.s3UploadRequest.on('httpHeaders', function (statusCode, headers) {
        const floderList = []
        for (const key in headers) {
          if (key === 'x-amz-device-used') {
            self.upload.used = parseInt(headers[key])
          }
          if (key === 'x-amz-device-capacity') {
            self.upload.total = parseInt(headers[key])
          }
          self.upload.percentage = self.upload.used / self.upload.total

          if (key.split('x-amz-')[1] && key.split('x-amz-')[1] !== 'request-id' && key.split('x-amz-')[1] !== 'bucket-region') {
            floderList.push({
              label: (key.split('x-amz-')[1]).toUpperCase(),
              size: self.parternFileSize(headers[key]),
              byteNum: parseInt(headers[key])
            })
          }
        }
        self.curBucketObjList.forEach(item => {
          floderList.forEach(v => {
            if (decodeURIComponent(v.label) === item.name) {
              item.byteNum = v.byteNum
            }
          })
        })
      })
      return true
    },
    // 获取文件夹列表
    getNetworkDiskInfo (params) {
      const self = this
      self.cur_fileLength = 0
      self.curFolders = []
      self.allBuckets = []
      self.isNewFiles = false
      self.isRename = false
      self.isAdd = true
      self.fileName = self.$t('disk.addFolder')
      self.upload = {
        percentage: 0,
        used: 0,
        total: 0
      }
      self.files = []
      if (!self.awsS3Client) {
        return false
      }
      self.setState({
        attr: 'onLoading',
        val: true
      })
      self.$Api.system.updateWsToken({ cluster_uuid: self.cluster_uuid })
      self.s3UploadRequest = self.awsS3Client.listBuckets(params, function (err, data) {
        self.setState({
          attr: 'onLoading',
          val: false
        })
        if (err) {
          let netDiskInfo = null
          if ((typeof self.netDisk) === 'string') {
            netDiskInfo = JSON.parse(self.netDisk)
          } else {
            netDiskInfo = self.netDisk
          }
          if (netDiskInfo.useSSL === 'https') {
            self.dialogHttpsVisible = true
            self.address = netDiskInfo.endPoint
            self.https = `${netDiskInfo.useSSL}://${netDiskInfo.endPoint}:${parseInt(netDiskInfo.port)}`
          } else {
            self.$message({
              message: err,
              type: 'error',
              duration: 3000
            })
          }
        } else {
          if (params) {
            data.Buckets.forEach((item) => {
              const namePath = item.Name.split('/')
              const len = namePath.length
              const fileName = namePath[len - 1]
              const curPath = self.cur_buckets.length ? '/' + self.cur_buckets.join('/') + '/' : ''
              if (item.Name.indexOf(curPath) === 0 && item.Name !== curPath) {
                console.log(11111)
                self.files.push({
                  name: fileName === '' ? namePath[len - 2] : fileName,
                  size: item.size ? item.size : '-',
                  access: item.Access,
                  isSelected: true,
                  type: fileName === '' ? 'folder' : ((fileName).split('.'))[(fileName).split('.').length - 1],
                  mtime: self.parternTime(item.CreationDate),
                  path: item.Name,
                  StorageQuota: 0
                })
              }
            })
          } else {
            data.Buckets.forEach((item) => {
              self.curFolders.forEach(v => {
                if (decodeURIComponent(v.label) === item.Name.toUpperCase()) {
                  self.files.push({
                    name: item.Name,
                    size: v.size ? v.size : '-',
                    access: item.Access,
                    byteNum: v.byteNum,
                    isSelected: true,
                    type: item.Name.split('.')[item.Name.split('.').length] || 'folder',
                    mtime: self.parternTime(item.CreationDate),
                    StorageQuota: 0
                  })
                }
              })
            })
            self.curBucketObjList = self.files
          }
          self.allBuckets = self.files
          self.cur_fileLength = self.files.length
          if (self.isFromVolume) {
            self.files.forEach(item => {
              if (!item.path || !item.path.split('/')[2]) {
                self.awsS3Client.getBucketQuota({ Bucket: item.name }, function (err, data) {
                  if (err) {
                    self.$message({
                      message: err,
                      type: 'error',
                      duration: 3000
                    })
                  } else {
                    item.StorageQuota = data.StorageQuota
                  }
                })
              }
            })
          }
        }
      })
      self.s3UploadRequest.on('httpHeaders', function (statusCode, headers) {
        for (const key in headers) {
          if (key === 'x-amz-device-used') {
            self.upload.used = parseInt(headers[key])
          }
          if (key === 'x-amz-device-capacity') {
            self.upload.total = parseInt(headers[key])
          }
          self.upload.percentage = self.upload.used / self.upload.total
        }
        for (const key in headers) {
          if (key.split('x-amz-')[1] && key.split('x-amz-')[1] !== 'request-id' && key.split('x-amz-')[1] !== 'bucket-region') {
            self.curFolders.push({
              label: (key.split('x-amz-')[1]).toUpperCase(),
              size: self.parternFileSize(headers[key]),
              byteNum: parseInt(headers[key])
            })
          }
        }
      })
    },
    // 列表选择
    handleSelectionChange (val) {
      const self = this
      self.networkDataSelect = []
      self.fileSelect = 0
      self.isSelected = false
      if (val && val.length > 0) {
        val.forEach(item => {
          if (item.type === 'folder') {
            self.isSelected = true
          }
          self.networkDataSelect.push(item)
        })
        self.isBoxShaow = true
        self.fileSelect = val.length
      } else {
        self.isBoxShaow = false
      }
    },
    // 离开拖拽区域
    handleDragLeave (ev) {
      ev.stopPropagation()
      ev.preventDefault()
      this.isUpload = false
      document.getElementById('dragable').style.cssText = 'z-index:0'
    },
    // 进入拖拽区域
    handleDrag (e) {
      e.stopPropagation()
      e.preventDefault()
      if (this.isShow) {
        document.getElementById('dragable').style.cssText = 'z-index:9999;border: 2px dashed #3583E3'
        this.isUpload = true
      }
    },
    // 获取拖拽信息
    handleDrop (e) {
      const self = this
      e.stopPropagation()
      e.preventDefault()
      if (this.isShow) {
        self.dragFiles = []
        var fileList = e.dataTransfer.files
        document.getElementById('dragable').style.cssText = 'z-index:0'
        var promise1 = new Promise(function (resolve, reject) {
          if (fileList.length) {
            const items = event.dataTransfer.items
            if (items && items.length && items[0].webkitGetAsEntry != null) {
              addFilesItems(items)
            }
          }
          // 区分拖拽的文件类型
          function addFilesItems (items) {
            for (var i = 0; i < items.length; i++) {
              var item = items[i]
              var entry = null
              if (item.webkitGetAsEntry && (entry = item.webkitGetAsEntry())) {
                if (entry.isFile) {
                  // 判定为文件
                  self.dragFiles.push(fileList[i])
                } else if (entry.isDirectory) {
                  // 判定为文件夹
                  addFilesFormDirectory(entry, entry.name)
                }
              }
            }
          }
          // 读取文件夹下的文件
          function addFilesFormDirectory (directory, path) {
            const dirReader = directory.createReader()
            dirReader.readEntries(function (entries) {
              entries.forEach(function (entry) {
                if (entry.isFile) {
                  // 如果是文件
                  entry.file(function (file) {
                    file.fullPath = path + '/' + file.name
                    self.dragFiles.push(file)
                  })
                } else if (entry.isDirectory) {
                  // 递归处理
                  addFilesFormDirectory(entry, path + '/' + entry.name)
                }
              })
            })
          }
          resolve(self.dragFiles)
        })
        promise1.then(function (data) {
          setTimeout(function () {
            self.dragUploadFile()
          }, 100)
        })
      } else {
        this.$message({
          message: self.$t('disk.enterFolderTip'),
          type: 'warning',
          duration: 3000
        })
      }
    },
    // 新建文件夹
    handleNetworkBoxCreate () {
      const self = this
      // if(!self.isRename){
      const cur_date = new Date()
      if (self.isAdd) {
        self.files.unshift({
          name: self.fileName,
          type: 'folder',
          size: '-',
          newCreat: true,
          mtime: '-'
        })
      }
      if (self.files.length - self.cur_fileLength > 0) {
        self.isAdd = false
      }
      // }
      self.isNewFiles = true
    },
    // 确认新建文件夹
    confirmCreatNewFiles () {
      const self = this
      if (!self.fileName) {
        self.$message({
          message: self.$t('disk.emptyfilename'),
          type: 'error',
          duration: 3000
        })
        return
      } else {
        var re = new RegExp("[`~!@#$^&*()=|{}':;',\\[\\].<>/?~！@#￥……&*（）——|{}【】‘；：”“'。，、？]")
        if (re.test(self.fileName)) {
          self.$message({
            message: self.$t('disk.specialName'),
            type: 'error',
            duration: 3000
          })
          return
        } else {
          if (self.fileName.length > 32) {
            self.$message({
              message: self.$t('disk.lengthName'),
              type: 'error',
              duration: 3000
            })
            return
          }
        }
      }
      let isSubmit = false
      if (!self.isAdd) {
        self.files.slice(1, self.files.length).forEach(item => {
          if (item.name === self.fileName) {
            isSubmit = true
          }
        })
      } else {
        self.files.forEach(item => {
          if (item.name === self.fileName) {
            isSubmit = true
          }
        })
      }
      if (!isSubmit) {
        self.setState({
          attr: 'onLoading',
          val: true
        })
        self.$Api.system.updateWsToken({ cluster_uuid: self.cluster_uuid || self.curClusterUuid })
        var params = {}
        if (!self.isRename) {
          if (!self.isShow) {
            sessionStorage.setItem('isNewBucket', true)
            self.awsS3Client.createBucket({ Bucket: self.fileName }, function (err) {
              sessionStorage.setItem('isNewBucket', '')
              self.setState({
                attr: 'onLoading',
                val: false
              })
              if (err) {
                self.$message({
                  message: err & err.message.includes('Access Denied') ? self.$t('disk.noPermission') : err,
                  type: 'error',
                  duration: 3000
                })
              } else {
                self.$message({
                  message: self.$t('disk.createfile'),
                  type: 'success',
                  duration: 3000
                })
                self.getNetworkDiskInfo()
                self.cancelCreatNewFiles()
              }
            })
          } else {
            let PrefixKey = ''
            PrefixKey = self.cur_buckets.slice(1, (self.cur_buckets.length))
            params = {
              Bucket: self.cur_buckets[0],
              Key: PrefixKey.length > 0 ? PrefixKey.join('/') + '/' + self.fileName + '/' : self.fileName + '/',
              FileType: 'dir'
            }
            self.awsS3Client.copyObject(params, function (err, data) {
              self.setState({
                attr: 'onLoading',
                val: false
              })
              if (err) {
                self.$message({
                  message: err && err.message.includes('Access Denied') ? self.$t('disk.noPermission') : err,
                  type: 'error',
                  duration: 3000
                })
              } else {
                self.$message({
                  message: self.$t('disk.createfile'),
                  type: 'success',
                  duration: 3000
                })
                self.getBucketObject()
                self.cancelCreatNewFiles()
              }
            })
          }
        } else {
          if (self.selectedFileObj.name === self.fileName) {
            self.$message({
              message: self.$t('disk.noEditName'),
              type: 'warning',
              duration: 3000
            })
            self.setState({
              attr: 'onLoading',
              val: false
            })
            return
          }
          if (self.selectedFileObj.type === 'folder') {
            params = {
              Bucket: self.isShow ? self.getFilePath(self.selectedFileObj.path) + '/' + self.fileName : self.fileName,
              Key: '',
              MoveSource: `${encodeURI(self.getFilePath(self.selectedFileObj.path))}/${encodeURI(self.selectedFileObj.name)}`
            }
          } else {
            params = {
              Bucket: self.getFilePath(self.selectedFileObj.path),
              Key: self.fileName,
              MoveSource: `/${encodeURI(self.getFilePath(self.selectedFileObj.path))}/${encodeURI(self.selectedFileObj.name)}`,
              FileType: self.fileName.split('.')[self.fileName.split('.').length - 1]
            }
          }
          self.awsS3Client.copyObject(params, function (err, data) {
            self.setState({
              attr: 'onLoading',
              val: false
            })
            if (err) {
              self.$message({
                message: err && err.message.includes('Access Denied') ? self.$t('disk.noPermission') : err,
                type: 'error',
                duration: 3000
              })
            } else {
              self.$message({
                message: `${self.selectedFileObj.name}` + self.$t('disk.renameSuccess'),
                type: 'success',
                duration: 3000
              })
              if (self.cur_buckets.length > 0) {
                self.getBucketObject()
              } else {
                self.getNetworkDiskInfo()
              }
              self.cancelRenameFiles()
            }
          })
        }
      } else {
        self.$message({
          message: `${self.fileName}` + self.$t('disk.exist'),
          type: 'warning',
          duration: 3000
        })
      }
    },
    // 取消新建文件夹
    cancelCreatNewFiles () {
      this.isNewFiles = false
      this.fileName = this.$t('disk.addFolder')
      if (!this.isRename) {
        this.files.splice(0, 1)
      }
      this.isAdd = true
    },
    // 取消重命名
    cancelRenameFiles () {
      this.isNewFiles = false
      this.isRename = false
      this.fileName = this.$t('disk.addFolder')
      this.files.forEach(item => {
        item.newCreat = false
      })
    },
    // 下载文件
    async handleNetworkDowanload () {
      const self = this
      const downloadWorks = self.networkDataSelect // 需要下载的任务
      const bucketPath = []
      self.$Api.system.updateWsToken({ cluster_uuid: self.cluster_uuid || self.curClusterUuid })
      const uploadContent = []
      downloadWorks.forEach(row => {
        const bucketsPath = self.getFilePath(row.path).split('/') // 当前下载任务的路径
        !bucketsPath[0] && (bucketsPath[0] = row.name) // 下载桶时指定桶名称
        if (row.type === 'folder') {
          let params = {}
          let PrefixKey = ''
          if (self.isShow || bucketPath.length > 0) {
            if (bucketPath.length === 0) {
              PrefixKey = bucketsPath.slice(1, (bucketsPath.length))
              params = {
                Bucket: bucketsPath[0],
                Prefix: PrefixKey.length > 0 ? PrefixKey.join('/') + '/' + row.name + '/' : PrefixKey.join('/') + row.name + '/'
              }
            } else {
              bucketPath.forEach(item => {
                PrefixKey = item
              })
              params = {
                Bucket: bucketsPath.length > 0 ? bucketsPath[0] : row.name,
                Prefix: PrefixKey
              }
            }
          } else {
            params = {
              Bucket: row.name
            }
          }
          const prefix = params.Prefix ? `/${params.Prefix}` : ''
          uploadContent.push(`/${params.Bucket}${prefix}`)
        } else {
          var params = {
            Bucket: bucketsPath.join('/'),
            Key: row.name // 要下载的文件名称
          }
          uploadContent.push(`/${bucketsPath.join('/')}/${row.name}`)
        }
      })
      const uplodBuckteUuid = await this.getDownBucketsUuid(uploadContent)
      this.downLoadBucketFile(uplodBuckteUuid)
    },

    handleNetworkDowanloadSingle (row) {
      const bucketPath = []
      const uploadContent = []
      let downloadWorks = [] // 需要下载的任务
      row ? downloadWorks.push(row) : downloadWorks = this.networkDataSelect
      this.$Api.system.updateWsToken({ cluster_uuid: this.cluster_uuid || this.curClusterUuid })
      downloadWorks.forEach(row => {
        const bucketsPath = this.getFilePath(row.path).split('/') // 当前下载任务的路径
        !bucketsPath[0] && (bucketsPath[0] = row.name) // 下载桶时指定桶名称
        if (row.type === 'folder') {
          let params = {}
          let PrefixKey = ''
          if (this.isShow || bucketPath.length > 0) {
            if (bucketPath.length === 0) {
              PrefixKey = bucketsPath.slice(1, (bucketsPath.length))
              params = {
                Bucket: bucketsPath[0],
                Prefix: PrefixKey.length > 0 ? PrefixKey.join('/') + '/' + row.name + '/' : PrefixKey.join('/') + row.name + '/'
              }
            } else {
              bucketPath.forEach(item => {
                PrefixKey = item
              })
              params = {
                Bucket: bucketsPath.length > 0 ? bucketsPath[0] : row.name,
                Prefix: PrefixKey
              }
            }
          } else {
            params = {
              Bucket: row.name
            }
          }
          const prefix = params.Prefix ? `/${params.Prefix}` : ''
          uploadContent.push(`/${params.Bucket}${prefix}`)
          this.downloadSingleFile(uploadContent, 2, row.name)
        } else {
          var params = {
            Bucket: bucketsPath.join('/'),
            Key: row.name // 要下载的文件名称
          }
          this.downloadSingleFile(params, 1, row.name)
        }
      })
    },
    async downloadSingleFile (params, type, fileName) {
      if (type == 2) {
        const uplodBuckteUuid = await this.getDownBucketsUuid(params)
        this.downLoadBucketFile(uplodBuckteUuid, fileName)
      } else {
        this.awsS3Client.getSignedUrl('getObject', params, (err, data) => {
          if (err) {
            this.$message({
              message: '获取下载链接失败: ' + this.errTxt[err.code] || err,
              type: 'error',
              duration: 1500
            })
            return
          }
          // const elemIF = document.createElement('a')
          // elemIF.href = data
          // elemIF.download = fileName
          // elemIF.style.display = 'none'
          // console.log('下载的文件', elemIF)
          // document.body.appendChild(elemIF)
          // elemIF.click()
          // document.body.removeChild(elemIF)
          const elemIF = document.createElement('iframe')
          elemIF.src = data
          elemIF.style.display = 'none'
          document.body.appendChild(elemIF)
        })
      }
    },
    getDownBucketsUuid (uploadContent) { // 获取s3下载的uuid
      const netDisk = typeof (this.netDisk) === 'string' ? JSON.parse(this.netDisk) : this.netDisk
      const params = {
        SrcsPath: uploadContent,
        s3Url: `${netDisk.useSSL}://${netDisk.endPoint}:${netDisk.port}`
      }
      console.log(netDisk, params)
      return new Promise(resolve => {
        this.$Api.netdisk.downLoadBucketUuid(params).then(res => {
          resolve(res.uuid)
        })
      })
    },
    downLoadBucketFile (uuid, fileName) { // 下载s3文件zip
      const netDisk = typeof (this.netDisk) === 'string' ? JSON.parse(this.netDisk) : this.netDisk
      this.$Api.netdisk.downLoadBucket({ uuid: uuid, s3Url: `${netDisk.useSSL}://${netDisk.endPoint}:${netDisk.port}` }).then(res => {
        var blob = new Blob([res], { type: 'application/x-tar' })
        if (window.navigator && window.navigator.msSaveOrOpenBlob) { // 兼容ie
          window.navigator.msSaveBlob(blob, fileName)
        } else {
          let linkNode = document.createElement('a')
          linkNode.download = new Date().Format('yyyy-MM-dd hhmmss')
          // linkNode.download = fileName + '.tar'
          linkNode.style.display = 'none'
          linkNode.href = URL.createObjectURL(blob)
          document.body.appendChild(linkNode)
          linkNode.click()
          URL.revokeObjectURL(linkNode.href)
          document.body.removeChild(linkNode)
        }
      })
    },

    // 原使用的下载文件
    // handleNetworkDowanload (row) {
    //   const self = this
    //   console.log(row)
    //   console.log(self.networkDataSelect)
    //   let downloadWorks = [] // 需要下载的任务
    //   row ? downloadWorks.push(row) : downloadWorks = self.networkDataSelect
    //   const bucketPath = []
    //   self.folderFiles = []
    //   self.$Api.system.updateWsToken({ cluster_uuid: self.cluster_uuid || self.curClusterUuid })
    //   downloadWorks.forEach(row => {
    //     const bucketsPath = self.getFilePath(row.path).split('/') // 当前下载任务的路径
    //     !bucketsPath[0] && (bucketsPath[0] = row.name) // 下载桶时指定桶名称
    //     if (row.type === 'folder') {
    //       var promise1 = new Promise(function (resolve, reject) {
    //         dirFoundFile()
    //         /* eslint-disable-next-line */
    //         function dirFoundFile (name) {
    //           let params = {}
    //           let PrefixKey = ''
    //           if (self.isShow || bucketPath.length > 0) {
    //             if (bucketPath.length === 0) {
    //               PrefixKey = bucketsPath.slice(1, (bucketsPath.length))
    //               params = {
    //                 Bucket: bucketsPath[0],
    //                 Prefix: PrefixKey.length > 0 ? PrefixKey.join('/') + '/' + row.name + '/' : PrefixKey.join('/') + row.name + '/'
    //               }
    //             } else {
    //               bucketPath.forEach(item => {
    //                 PrefixKey = item
    //               })
    //               params = {
    //                 Bucket: bucketsPath.length > 0 ? bucketsPath[0] : row.name,
    //                 Prefix: PrefixKey
    //               }
    //             }
    //           } else {
    //             params = {
    //               Bucket: row.name
    //             }
    //           }
    //           self.awsS3Client.listObjects(params, function (err, data) {
    //             if (err) {
    //               self.$message({
    //                 message: err,
    //                 type: 'error',
    //                 duration: 3000
    //               })
    //             } else {
    //               data.Contents.forEach(item => {
    //                 self.folderFiles.push({
    //                   bucket: data.Prefix,
    //                   name: item.Key,
    //                   totalBucket: data.Name,
    //                   type: item.Key.split('.')[item.Key.split('.').length - 1] || ''
    //                 })
    //               })
    //               data.CommonPrefixes.forEach(item => {
    //                 bucketPath.push(item.Prefix)
    //                 dirFoundFile()
    //               })
    //             }
    //           })
    //         }
    //         resolve(self.folderFiles)
    //       })
    //       promise1.then(function (data) {
    //         var params = {}
    //         setTimeout(function () {
    //           data.forEach(v => {
    //             params = {
    //               Bucket: v.totalBucket + '/' + v.bucket,
    //               Key: v.name // 要下载的文件名称
    //             }
    //             downloadFiles(params, v)
    //           })
    //           if (!data.length) {
    //             self.$message({
    //               message: self.$t('disk.emptyFolderTip'),
    //               type: 'warning',
    //               duration: 3000
    //             })
    //           }
    //         }, 1000)
    //       })
    //     } else {
    //       var params = {
    //         Bucket: bucketsPath.join('/'),
    //         Key: row.name // 要下载的文件名称
    //       }
    //       downloadFiles(params, row)
    //     }
    //   })
    //   // !row && self.handleSelectionChange()

    //   function downloadFiles (params, row) {
    //     self.awsS3Client.getSignedUrl('getObject', params, function (err, data) {
    //       if (err) {
    //         self.$message({
    //           message: '获取下载链接失败: ' + self.errTxt[err.code] || err,
    //           type: 'error',
    //           duration: 3000
    //         })
    //         return
    //       }
    //       var elemIF = document.createElement('iframe')
    //       elemIF.src = data
    //       elemIF.style.display = 'none'
    //       document.body.appendChild(elemIF)
    //     })
    //   }
    // },
    // handleNetworkDowanload (row) {
    //   this.dialogDownloadVisible = true
    //   this.curFile = row
    //   if (this.dialogDownloadVisible) { return }
    //   const self = this
    //   console.log(row)
    //   console.log(self.networkDataSelect)
    //   let downloadWorks = [] // 需要下载的任务
    //   row ? downloadWorks.push(row) : downloadWorks = self.networkDataSelect
    //   const bucketPath = []
    //   self.folderFiles = []
    //   self.$Api.system.updateWsToken({ cluster_uuid: self.cluster_uuid || self.curClusterUuid })
    //   downloadWorks.forEach(row => {
    //     const bucketsPath = self.getFilePath(row.path).split('/') // 当前下载任务的路径
    //     !bucketsPath[0] && (bucketsPath[0] = row.name) // 下载桶时指定桶名称
    //     if (row.type === 'folder') {
    //       var promise1 = new Promise(function (resolve, reject) {
    //         dirFoundFile()
    //         /* eslint-disable-next-line */
    //         function dirFoundFile (name) {
    //           let params = {}
    //           let PrefixKey = ''
    //           if (self.isShow || bucketPath.length > 0) {
    //             if (bucketPath.length === 0) {
    //               PrefixKey = bucketsPath.slice(1, (bucketsPath.length))
    //               params = {
    //                 Bucket: bucketsPath[0],
    //                 Prefix: PrefixKey.length > 0 ? PrefixKey.join('/') + '/' + row.name + '/' : PrefixKey.join('/') + row.name + '/'
    //               }
    //             } else {
    //               bucketPath.forEach(item => {
    //                 PrefixKey = item
    //               })
    //               params = {
    //                 Bucket: bucketsPath.length > 0 ? bucketsPath[0] : row.name,
    //                 Prefix: PrefixKey
    //               }
    //             }
    //           } else {
    //             params = {
    //               Bucket: row.name
    //             }
    //           }
    //           self.awsS3Client.listObjects(params, function (err, data) {
    //             if (err) {
    //               self.$message({
    //                 message: err,
    //                 type: 'error',
    //                 duration: 3000
    //               })
    //             } else {
    //               data.Contents.forEach(item => {
    //                 self.folderFiles.push({
    //                   bucket: data.Prefix,
    //                   name: item.Key,
    //                   totalBucket: data.Name,
    //                   type: item.Key.split('.')[item.Key.split('.').length - 1] || ''
    //                 })
    //               })
    //               data.CommonPrefixes.forEach(item => {
    //                 bucketPath.push(item.Prefix)
    //                 dirFoundFile()
    //               })
    //             }
    //           })
    //         }
    //         resolve(self.folderFiles)
    //       })
    //       promise1.then(function (data) {
    //         var params = {}
    //         setTimeout(function () {
    //           data.forEach(v => {
    //             params = {
    //               Bucket: v.totalBucket + '/' + v.bucket,
    //               Key: v.name // 要下载的文件名称
    //             }
    //             downloadFiles(params, v)
    //           })
    //           if (!data.length) {
    //             self.$message({
    //               message: '文件夹为空！',
    //               type: 'warning',
    //               duration: 3000
    //             })
    //           }
    //         }, 1000)
    //       })
    //     } else {
    //       var params = {
    //         Bucket: bucketsPath.join('/'),
    //         Key: row.name // 要下载的文件名称
    //       }
    //       downloadFiles(params, row)
    //     }
    //   })
    //   // !row && self.handleSelectionChange()

    //   function downloadFiles (params, row) {
    //     self.awsS3Client.getSignedUrl('getObject', params, function (err, data) {
    //       if (err) {
    //         self.$message({
    //           message: '获取下载链接失败: ' + err,
    //           type: 'error',
    //           duration: 3000
    //         })
    //         return
    //       }
    //       var elemIF = document.createElement('iframe')
    //       elemIF.src = data
    //       elemIF.style.display = 'none'
    //       document.body.appendChild(elemIF)
    //     })
    //   }
    // },
    confirmDownload (obj) {
      this.dialogDownloadVisible = false
      this.$set(this.curFile, 'percentage', 0)
      this.$set(this.curFile, 'isSuccess', true)
      this.$set(this.curFile, 'isDownloading', true)
      this.$set(this.curFile, 'path', obj.path)
      this.download_fileList.unshift(this.curFile)
    },
    // 下载判断文件类型
    testFileType (name) {
      const type = name.slice(name.lastIndexOf('.') + 1)
      if (this.fileTypes['img'].indexOf(type) > 0) {
        return 'image/' + type
      } else {
        return 'text/' + type === 'html' ? 'html' : 'plain'
      }
    },
    // 分享
    handleNetworkShare (row) {
      this.dialogShareVisible = true
      this.shareFiles = row
    },
    // 复制到
    handleNetworkBoxCopy (row, type) {
      this.dialogCopyVisible = true
      this.copyFileObj = row
      this.copyType = type
      this.geBucketList()
    },
    //  批量复制
    handleNetworkCopyMultiple (type) {
      this.dialogCopyVisible = true
      this.batchSelectObj = this.networkDataSelect
      this.copyType = type
      this.geBucketList()
    },
    // 重命名
    handleNetworkBoxRename (row) {
      if (row.access == 'READ') {
        this.$message({
          message: this.$t('disk.noPermission'),
          type: 'error',
          duration: 3000
        })
        return false
      }
      this.selectedFileObj = row
      this.isNewFiles = true
      this.isRename = true
      this.fileName = row.name
      this.files.forEach(item => {
        item.newCreat = false
        if (item.name === row.name) {
          item.newCreat = true
        }
      })
    },
    // 移动
    handleNetworkBoxMove (row, type) {
      this.moveFileObj = row
      this.dialogMoveVisible = true
      this.moveType = type
      this.geBucketList()
    },
    // 批量移动
    handleNetworkMoveMultiple (type) {
      this.dialogMoveVisible = true
      this.batchSelectObj = this.networkDataSelect
      this.moveType = type
      this.geBucketList()
    },
    getNetDiskFilesOpt (obj) {
      this.netDiskOpttVisible = obj.netDiskOpttVisible
      this.netDiskFilesOptType = obj.type
      this.optFiles = obj.optObject
      this.optSameFiles = obj.sameObject
      this.dialogCopyVisible = false
      this.dialogMoveVisible = false
    },
    // 覆盖所有文件
    coverAllFilesOpt () {
      if (this.netDiskFilesOptType === 1) {
        this.$refs.copyObject.coverAllFilesOpt()
      } else {
        this.$refs.moveObject.coverAllFilesOpt()
      }
      this.$refs.netDiskTable.clearSelection()
    },
    // 跳过同名文件
    jumpAllFilesOpt () {
      if (this.netDiskFilesOptType === 1) {
        this.$refs.copyObject.jumpAllFilesOpt()
      } else {
        this.$refs.moveObject.jumpAllFilesOpt()
      }
      this.$refs.netDiskTable.clearSelection()
    },
    // 查看文件夹
    checknetworkDiskFiels (row, column) {
      const self = this
      if ((column.label === '文件名' || column.label === 'File name') && row.type === 'folder' && !self.isNewFiles) {
        self.$Api.system.updateWsToken({ cluster_uuid: self.cluster_uuid || self.curClusterUuid })
        self.pagenum = 1
        self.total = 0
        if (!self.isShow) {
          self.cur_file_auth = row.access //  获取当前文件夹的权限 只读/读写
        }
        self.isShow = true
        if (self.cur_buckets.length === 0) {
          self.cur_buckets.push(row.name)
        } else {
          self.cur_buckets.push(row.name)
        }
        if (row.path) { self.cur_buckets = row.path.slice(1, row.path.length - 1).split('/') }
        self.setState({
          attr: 'onLoading',
          val: true
        })
        // eslint-disable-next-line standard/object-curly-even-spacing
        this.loadingHidden = this.$loading({ target: '.netDisk-page'})
        self.getBucketObject()
      }
    },
    // 返回存储卷
    // backBeforeVolume () {
    //   const self = this
    //   self.$router.push('/pages/storage/volume')
    // },
    // 格式化时间
    parternTime (Date) {
      return Date.getFullYear() +
            '-' +
          ((Date.getMonth() + 1) < 10 ? ('0' + (Date.getMonth() + 1)) : (Date.getMonth() + 1)) +
          '-' +
          (Date.getDate() < 10 ? ('0' + Date.getDate()) : (Date.getDate())) +
          ' ' +
          (Date.getHours() < 10 ? ('0' + Date.getHours()) : Date.getHours()) +
          ':' +
          (Date.getMinutes() < 10 ? ('0' + Date.getMinutes()) : Date.getMinutes()) +
          ':' +
          (Date.getSeconds() < 10 ? ('0' + Date.getSeconds()) : Date.getSeconds())
    },
    // 格式化文件大小
    parternFileSize (size) {
      if (size > 1024 && size < 1024 * 1024) {
        return parseFloat(size / 1024).toFixed(1) + 'KB'
      } else if (size > 1024 * 1024 && size < 1024 * 1024 * 1024) {
        return parseFloat(size / (1024 * 1024)).toFixed(1) + 'M'
      } else if (size > 1024 * 1024 * 1024) {
        return parseFloat(size / (1024 * 1024 * 1024)).toFixed(1) + 'G'
      } else if (size > 0) {
        return size + 'B'
      } else {
        return '-'
      }
    },
    cancelNetDiskInfo () {
      this.allBuckets = []
      this.batchSelectObj = []
      this.copyFileObj = null
      this.moveFileObj = null
      this.$refs.netDiskTable.clearSelection()
    },
    // 获取文件夹列表
    geBucketList () {
      const self = this
      self.allBuckets = []
      self.setState({
        attr: 'innerLoading',
        val: true
      })
      self.$Api.system.updateWsToken({ cluster_uuid: self.cluster_uuid || self.curClusterUuid })
      self.awsS3Client.listBuckets(function (err, data) {
        self.setState({
          attr: 'innerLoading',
          val: false
        })
        if (err) {
          self.$message({
            message: err,
            type: 'error',
            duration: 3000
          })
        } else {
          let directory = {}
          data.Buckets.forEach((item) => {
            directory = {
              name: item.Name,
              type: item.StorageClass || 'folder',
              size: '-',
              isSelected: true,
              mtime: self.parternTime(item.CreationDate)
            }
            if (self.moveFileObj) {
              if (self.moveFileObj.name !== item.Name) {
                self.allBuckets.push(directory)
              } else if (self.isShow) {
                self.allBuckets.push(directory)
              }
            } else if (self.copyFileObj) {
              if (self.copyFileObj.name !== item.Name) {
                self.allBuckets.push(directory)
              } else if (self.isShow) {
                self.allBuckets.push(directory)
              }
            } else {
              self.allBuckets.push(directory)
            }
          })
        }
      })
    },
    // 文件夹路径跳转
    goBucketPath (row, index) {
      const self = this
      if ((index + 1) === self.cur_buckets.length) {
        return
      }
      self.isNewFiles = false
      self.isRename = false
      self.pagenum = 1
      self.total = 0
      self.cur_buckets.splice((index + 1), (self.cur_buckets.length - index - 1))
      self.getBucketObject(self.cur_buckets, true)
    },
    // 文件夹下文件分页
    handleSizeChange (val) {
      this.$nextTick(() => {
        this.pagesize = val
        this.getBucketObject()
      })
    },
    // 文件夹下文件分页
    handleCurrentChange (val) {
      this.$nextTick(() => {
        this.pagenum = val
        this.getBucketObject()
      })
    },

    // 获取文件图标
    getFileIcon (type) {
      let fileType = '' // 文件的类型
      for (const key in this.fileTypes) {
        if (this.fileTypes[key].indexOf(type) > -1) { fileType = key }
      }
      if (fileType) {
        return this.fileIcon[fileType]
      } else return this.fileIcon.weizhi
    },
    // 获取文件夹路径
    getFilePath (path) {
      if (!path) return this.cur_buckets.join('/')
      if (path.charAt(path.length - 1) === '/') return path.split('/').splice(1, path.split('/').length - 3).join('/')
      else return path.split('/').splice(1, path.split('/').length - 2).join('/')
    }
  },
  computed: {
    ...mapState('Netdisk', ['S3Obeject']),
    isFromVolume: function () {
      // return this.whereFrom === 'volume'
      return true
    }
  }
}
</script>
<style lang="scss" scoped>
/deep/.el-table__header-wrapper{
      height: 40px !important;
}
</style>
<style lang="scss">
  .oprateIcon{
    display: inline-block;
    padding: 1px 6px;
    color: #3583E3;
  }
  /deep/ .el-dropdown-menu__item:hover{
    color: #3583E3 !important;
  }
  .pathBox{
    display: inline-block;
    max-width:calc(100% - 300px);
    overflow: auto;
    white-space: nowrap;
    vertical-align: top;
    &::-webkit-scrollbar {
      width: 5px;
      height: 5px;
    }

    /* 滚动槽 */
    &::-webkit-scrollbar-track {
      border-radius: 10px;
      background: #ededed;
    }

    &::-webkit-scrollbar-thumb {
      border-radius: 10px;
      background: #bbb5b5;
    }
  }
  .netDisk-icon{
    width:30px;
    height:30px;
    vertical-align: middle;
  }
  // .icon_colorful{
  //   color: #3583E3;
  // }
  .el-button-group{
    [id^="volume_networkBox_"]{
      margin:0
    }
  }
  .f-right{
    // margin-top: -3px;
    float: right;
    .el-input-group__append{
      width: 30px;
      .el-button{
        margin:0
      }
    }
    .el-icon-s-unfold{
      font-size: 28px;
    }
    .networkBox_showMode{
      display: inline-block;
      vertical-align: middle;
      >i{
        font-size: 28px;
      }
    }
  }
  .networkBox_list--mode{
    position: relative;
    .boxShaow{
      position: absolute;
      line-height: 36px;
      width: calc(100% - 55px);
      top: 32px;
      left:55px;
      z-index: 3;
      background: #F3F3F3;
    }
  }
  .el-input-group--append{
    margin-right: 10px;
    .el-input-group__append{
      padding: 0;
      &:hover{
        border-color: #3583E3;
      }
      .el-button{
        color: #3583E3 !important;
        &:hover{
          border-left-color: #3583E3;
        }
      }
    }
  }
  .netDisk-page{
    height: 100%;
    margin-left: 15px;
    .vx-box-header{
      z-index: 5;
      .el-button{
        margin-left: 0;
      }
      .selectType{
        width: 300px;
      }
    }
  }
  .networkBox_list--num{
      color:#323132;
      font-weight:600;
  }
  .volume_networkBox_list--createFile{
    >div{
      width: 50%;
    }
    >span{
      display: inline-block;
      vertical-align: middle;
      margin-left:10px;
      font-size: 18px;
    }
  }
  .fileError{
    color:#df1130;
    font-size: 12px !important;
  }
  .networkDisk-dropdown-menu.el-dropdown-menu /deep/ .el-dropdown-menu__item{
    color:#323132;
    &.is-disabled{
      color:#ccc !important
    }
  }
  @media screen and(max-width:1439px) {
    .netDisk-progress{
      right:30px;
    }
    .upload-demo{
      top:150px;
    }
  }
  @media screen and(min-width:1439px) {
    .netDisk-progress{
      right:40px;
    }
    .upload-demo{
      top:180px;
    }
  }
  .netDisk-progress{
    position: absolute;
    width: 310px;
    height: calc(100% - 220px) !important;
    margin-bottom: 0 !important;
    top: 140px;
    right: 20px;
    background:#fff;
    border-radius:2px;
    border:1px solid rgba(247,245,245,1);
    z-index:9;
    th{
      background: #fff !important;
      .cell{
        background: #fff !important;
      }
    }
    .el-progress {
      width: 100%;
      top:3px;
    }
    /deep/.el-progress-circle{
      width: 40px !important;
      height: 40px !important;
    }
    /deep/.el-progress__text{
      font-size: 12px !important;
      color:#323132 !important;
      width: 40px !important;
    }
  }
  .f-right>.el-button{
    &:last-child{
      margin-right:0 !important;
    }
  }
  .upload-input{
    width: 70px;
    height: 28px;
    margin-left: -70px;
    opacity:0;
  }
  .upload-capacity{
    position:inherit;
    bottom:20px;
    width:calc(100% - 20px);
    padding:0 10px;
  }
  .goBucketPath{
    color:#3583E3;
    &:last-child{
      color:#323132;
    }
  }
  #dragable{
    position: fixed;
    width: calc(100% - 150px);
    top:101px;
    right: 150;
    height: calc(100% - 101px);
    background: #fff;
    opacity: 0.8;
    .dragable_upload--text{
      position: absolute;
      color:#666;
      font-size: 30px;
      text-align: center;
      top:50%;
      left:50%;
      transform: translate(-50%)
    }
  }
  .Https-content{
    padding:10px;
    height: 240px;
    .Https-notice{
      display: inline-block;
      vertical-align: middle;
      font-size:24px;
      font-family:PingFang-SC;
      font-weight:bold;
      color:rgba(50,49,50,1);
      line-height:33px;
      margin-bottom: 40px;
    }
    .tab-img{
      display: inline-block;
      vertical-align: middle;
      margin-right: 20px;
    }
    .https-desc{
      font-size:16px;
      font-family:PingFangSC;
      font-weight:400;
      color:rgba(50,49,50,1);
      line-height:22px;
    }
    a{
      display: block;
      margin:20px 0;
      font-size:16px;
      font-family:PingFangSC;
      font-weight:400;
      color:rgba(74,144,226,1);
      line-height:22px;
      &:hover{
        color:#df1130;
      }
    }
    .Https-set{
      font-size:14px;
      font-family:PingFangSC;
      font-weight:400;
      color:rgba(50,49,50,1);
      line-height:20px;
    }
  }
  #volume_networkBox_uploadOpt /deep/ .el-dialog__body{
    padding-bottom: 20px;
    div{
      font-size:16px;
      font-family:PingFangSC;
      font-weight:400;
      color:rgba(50,49,50,1);
    }
    ul{
      margin-top: 10px;
    }
    li{
      line-height: 20px;
      margin:5px 0;
      cursor: pointer;
      color: #3583E3
    }
  }
  #volume_networkBox_move_copyOpt /deep/ .el-dialog__body{
    padding-bottom: 20px;
    div{
      font-size:16px;
      font-family:PingFangSC;
      font-weight:400;
      color:rgba(50,49,50,1);
    }
    ul{
      margin-top: 10px;
    }
    li{
      line-height: 20px;
      margin:5px 0;
      cursor: pointer;
      color: #3583E3
    }
  }

.block-desktop{
    text-align: center;
    margin-top: 18%;
    p{
      font-size: 15px;
      color: #323132;
      margin-top: 10px;
    }
}
</style>
